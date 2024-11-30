#include "addnewuserstackwidget.h"
#include "tcpnetworkconnection.h"
#include "addusernamecardwidget.h"
#include "ui_addnewuserstackwidget.h"
#include <useraccountmanager.hpp>
#include <authenticatenewfriendrequestdialog.h>
#include <QListWidgetItem>

AddNewUserStackWidget::AddNewUserStackWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AddNewUserStackWidget) {
  ui->setupUi(this);

  /*signal<->slot*/
  registerSignal();
}

AddNewUserStackWidget::~AddNewUserStackWidget() { delete ui; }

void AddNewUserStackWidget::addNewWidgetItem(std::shared_ptr<UserFriendRequest> info) {
  /*allocate memory*/
  AddUserNameCardWidget *namecard = new AddUserNameCardWidget;

  /*transfer ownership*/
  namecard->setNameCardInfo(info);

  /* when user click add friend */
  connect(namecard, &AddUserNameCardWidget::signal_add_friend, [this](std::shared_ptr<UserFriendRequest> info){
      qDebug() << "User agree to add this friend";
      AuthenticateNewFriendRequestDialog* auth = new AuthenticateNewFriendRequestDialog(this);
      auth->setModal(true);
      //
      auth->show();
  });

  /*create item for Qlistwidget*/
  QListWidgetItem *item = new QListWidgetItem;

  /*set size and attributes*/
  item->setSizeHint(namecard->sizeHint());
  item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);

  /*start to insert item*/
  ui->friends_list->insertItem(0, item);
  ui->friends_list->setItemWidget(item, namecard);  
}

void AddNewUserStackWidget::registerSignal()
{
    connect(TCPNetworkConnection::get_instance().get(), &TCPNetworkConnection::signal_incoming_friend_request,
            this, &AddNewUserStackWidget::slot_incoming_friend_request);

    connect(TCPNetworkConnection::get_instance().get(), &TCPNetworkConnection::signal_init_friend_request_list,
            this, &AddNewUserStackWidget::slot_init_friend_request_list);
}

void AddNewUserStackWidget::slot_init_friend_request_list(){
    auto friendRequest = UserAccountManager::get_instance()->getFriendRequestList();
    for(const auto &item: friendRequest){
        addNewWidgetItem(item);
    }
}

void AddNewUserStackWidget::slot_incoming_friend_request(std::optional<std::shared_ptr<UserFriendRequest>> info){
    if(info.has_value()){
        qDebug() << "Receive Friend Request From " << info.value()->m_uuid;
        /*did the friend request sender send the request before?*/
        if(UserAccountManager::get_instance()->alreadyExistInRequestList(info.value()->m_uuid)){
            return;
        }

        /*add it to UI interface*/
        addNewWidgetItem(info.value());

        /*add it to the list*/
        UserAccountManager::get_instance()->addItem2List(info.value());
    }
}

