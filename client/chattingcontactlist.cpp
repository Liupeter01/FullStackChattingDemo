#include "chattingcontactlist.h"
#include "chattingcontactitem.h"
#include "tools.h"
#include <QListWidgetItem>
#include <QRandomGenerator>
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

void ChattingContactList::slot_itemClicked(QListWidgetItem *item) {
  /*get widget base type*/
  ListItemWidgetBase *base =
      reinterpret_cast<ListItemWidgetBase *>(this->itemWidget(item));

  if (base == nullptr) {
    qDebug() << "ListItemWidgetBase is null!\n";
    return;
  }

  /*filter ListItemWidgetBase ItemType*/
  if (base->getItemType() == ListItemType::Default) {
    qDebug() << "invalid widget object!\n";
    return;
  }
  /*click Contact By Group*/
  else if (base->getItemType() == ListItemType::ShowContactByGroup) {
    qDebug() << "group widget is clicked, after click this we could hide the "
                "contact!\n";
    return;
  }
  /*click add user widget button*/
  else if (base->getItemType() == ListItemType::AddUserWidget) {
    qDebug() << "add user widget button is clicked turn to add user widget\n";
    // emit signal
    // emit;
  }
  /*click Contact widget item*/
  else if (base->getItemType() == ListItemType::ContactHistory) {
    qDebug() << "contact is clicked\n";
    // emit signal
    // emit;
  }
}

void ChattingContactList::loadContactsTest() {
  /*add new friend button as a group*/
  addGroupSeperator(static_text);
  addAddUserWidget();

  /*show contact as a group*/
  addGroupSeperator(QString("My Contact"));

  /*load test user avator image im "/static/" dir*/
  Tools::loadImgResources({"0.png", "1.png", "2.png", "3.png", "4.png", "5.png",
                           "6.png", "7.png", "8.png"},
                          ChattingContactItem::getImageSize().width(),
                          ChattingContactItem::getImageSize().height(),
                          "/static/");

  for (std::size_t i = 0; i < 10; ++i) {
    auto random1 = QRandomGenerator::global()->bounded(9);
    auto random2 = QRandomGenerator::global()->bounded(9);
    auto path = QString::number(random1) + ".png";
    qDebug() << "static path = /static/" << path;
    addChattingContact(path, QString::number(random2));
  }
}

void ChattingContactList::registerSignal() {
  /*user click one of the contact, connect signal<->slot*/
  connect(this, &QListWidget::itemClicked, this,
          &ChattingContactList::slot_itemClicked);
}
