#include "addnewuserstackwidget.h"
#include "tcpnetworkconnection.h"
#include "addusernamecardwidget.h"
#include "ui_addnewuserstackwidget.h"
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

  /*create item for Qlistwidget*/
  QListWidgetItem *item = new QListWidgetItem;

  /*set size and attributes*/
  item->setSizeHint(namecard->sizeHint());
  item->setFlags(item->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);

  /*start to insert item*/
  ui->friends_list->insertItem(0, item);
  ui->friends_list->setItemWidget(item, namecard);

  // TODO
}

void AddNewUserStackWidget::registerSignal()
{
    connect(TCPNetworkConnection::get_instance().get(), &TCPNetworkConnection::signal_incoming_friend_request, this, &AddNewUserStackWidget::slot_incoming_friend_request);
}

void AddNewUserStackWidget::slot_incoming_friend_request(std::optional<std::shared_ptr<UserFriendRequest>> info)
{
    if(info.has_value()){
        addNewWidgetItem(info.value());
        return;
    }
}

