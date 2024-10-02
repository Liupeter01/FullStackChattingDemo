#include "tools.h"
#include "chattingcontactlist.h"
#include "chattingcontactitem.h"
#include <QListWidgetItem>
#include <QScrollBar>
#include <QRandomGenerator>

ChattingContactList::ChattingContactList(QWidget *parent)
    : static_text("Add New Friend"), MainFrameShowLists(parent) {
  /*register signal*/
  registerSignal();

  /*load contact test func*/
  loadContactsTest();
}

ChattingContactList::~ChattingContactList() {}

void ChattingContactList::addAddUserWidget() {
  ChattingContactItem *add_widget(new ChattingContactItem);

  /*set AddUserWidget*/
  add_widget->setAddUserWidget();

  QListWidgetItem *item(new QListWidgetItem);
  item->setSizeHint(add_widget->sizeHint());

  this->addItem(item);
  this->setItemWidget(item, add_widget);

  /*set it by default*/
  this->setCurrentItem(item);
  this->update();
}

void ChattingContactList::addChattingContact(const QString &target_picture,
                                             const QString &text) {
  ChattingContactItem *contact_widget(new ChattingContactItem);

  /*set chatting contact info*/
  contact_widget->setChattingContact(target_picture, text);

  QListWidgetItem *item(new QListWidgetItem);
  item->setSizeHint(contact_widget->sizeHint());

  this->addItem(item);
  this->setItemWidget(item, contact_widget);
  this->update();
}

void ChattingContactList::addGroupSeperator(const QString &text) {
  ChattingContactItem *group_widget(new ChattingContactItem);

  /*set dialog with seperator*/
  group_widget->setGroupSeperator(text);

  QListWidgetItem *item(new QListWidgetItem);
  item->setSizeHint(group_widget->sizeHint());

  /*item should be non-clickable*/
  item->setFlags(item->flags() & ~Qt::ItemIsSelectable);

  this->addItem(item);
  this->setItemWidget(item, group_widget);
  this->update();
}

void ChattingContactList::slot_itemClicked(QListWidgetItem *item) {}

void ChattingContactList::loadContactsTest() {
  /*add new friend button as a group*/
  addGroupSeperator(static_text);
  addAddUserWidget();

  /*show contact as a group*/
  addGroupSeperator(QString("My Contact"));

  /*load test user avator image im "/static/" dir*/
  Tools::loadImgResources({"0.png","1.png","2.png","3.png","4.png","5.png","6.png","7.png","8.png"},
                          ChattingContactItem::getImageSize().width(),
                          ChattingContactItem::getImageSize().height(),
                          "/static/"
                          );

  for (std::size_t i = 0; i < 10; ++i) {
      auto random = QRandomGenerator::global()->bounded(9);
      auto path = QString::number(random) + ".png";
      qDebug() << "static path = /static/" << path;
      addChattingContact(path, QString::number(random));
  }
}

void ChattingContactList::registerSignal() {
  /*user click one of the contact, connect signal<->slot*/
  connect(this, &QListWidget::itemClicked, this,
          &ChattingContactList::slot_itemClicked);
}
