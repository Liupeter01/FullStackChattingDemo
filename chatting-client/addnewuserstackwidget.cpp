#include "addnewuserstackwidget.h"
#include "UserNameCard.h"
#include "addusernamecardwidget.h"
#include "ui_addnewuserstackwidget.h"
#include <QListWidgetItem>

AddNewUserStackWidget::AddNewUserStackWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::AddNewUserStackWidget) {
  ui->setupUi(this);

  /*test function*/
  loadWidgetTest();
}

AddNewUserStackWidget::~AddNewUserStackWidget() { delete ui; }

void AddNewUserStackWidget::addNewWidgetItem(
    std::unique_ptr<UserNameCard> info) {
  /*allocate memory*/
  AddUserNameCardWidget *namecard = new AddUserNameCardWidget;

  /*transfer ownership*/
  namecard->setNameCardInfo(std::move(info));

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

void AddNewUserStackWidget::loadWidgetTest() {
  std::unique_ptr<UserNameCard> info(
      std::make_unique<UserNameCard>(QString::number(0), "4.png", "test_name",
                                     "test_name", "test_desc", Sex::Male));
  /*transfer ownership*/
  addNewWidgetItem(std::move(info));
}
