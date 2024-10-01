#include "chattingcontactlist.h"
#include "chattingcontactitem.h"
#include <QListWidgetItem>
#include <QScrollBar>

ChattingContactList::ChattingContactList(QWidget *parent)
    : static_text("Add New Friend"), MainFrameShowLists(parent) {
  /*register signal*/
  registerSignal();

  /*load contact test func*/
  loadContactsTest();
}

ChattingContactList::~ChattingContactList() {}

void ChattingContactList::addAddUserWidget() {
  ChattingContactItem *item(new ChattingContactItem);

  /*set AddUserWidget*/
  item->setAddUserWidget();
  addItemWidget(item);
}

void ChattingContactList::addChattingContact(const QString &target_picture,
                                             const QString &text) {
  ChattingContactItem *item(new ChattingContactItem);

  /*set chatting contact info*/
  item->setChattingContact(target_picture, text);
  addItemWidget(item);
}

void ChattingContactList::addGroupSeperator(const QString &text) {
  ChattingContactItem *item(new ChattingContactItem);

  /*set dialog with seperator*/
  item->setGroupSeperator(text);
  addItemWidget(item);
}

void ChattingContactList::slot_itemClicked(QListWidgetItem *item) {}

void ChattingContactList::loadContactsTest() {
  /*add new friend button as a group*/
  addGroupSeperator(static_text);
  addAddUserWidget();

  /*show contact as a group*/
  addGroupSeperator(QString("My Contact"));
}

void ChattingContactList::registerSignal() {
  /*user click one of the contact, connect signal<->slot*/
  connect(this, &QListWidget::itemClicked, this,
          &ChattingContactList::slot_itemClicked);
}

void ChattingContactList::addItemWidget(ChattingContactItem *new_inserted) {
  if (new_inserted == nullptr) {
    return;
  }

  QListWidgetItem *item(new QListWidgetItem);

  item->setSizeHint(new_inserted->sizeHint());

  this->addItem(item);
  this->setItemWidget(item, new_inserted);
  this->update();
}
