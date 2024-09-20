#include "mainframesearchlists.h"
#include "addnewuserwidget.h"
#include <QListWidgetItem>

MainFrameSearchLists::MainFrameSearchLists(QWidget *parent)
    : MainFrameShowLists(parent) {
  /*regsiter signal slot*/
  registerSignal();

  /*add a startup widget inside the list*/
  addNewUserWidget();

  /*add style sheet for search_list and add user widget*/
  addStyleSheet();
}

MainFrameSearchLists::~MainFrameSearchLists() {}

void MainFrameSearchLists::registerSignal() {
  /*when user click searched item, the emit itemClicked signal*/
  connect(this, &QListWidget::itemClicked, this,
          &MainFrameSearchLists::slot_item_clicked);
}

void MainFrameSearchLists::addNewUserWidget() {
  AddNewUserWidget *new_inserted(new AddNewUserWidget());
  QListWidgetItem *item(new QListWidgetItem);
  item->setSizeHint(new_inserted->sizeHint());

  this->addItem(item);
  this->setItemWidget(item, new_inserted);
  this->update();
}

void MainFrameSearchLists::addStyleSheet() {
  this->setStyleSheet("#search_list{border:none;outline:none}");
  this->setStyleSheet("#search_list::item::selected{background-color:#d3d7d4;"
                      "border:none;outline:none}");
  this->setStyleSheet("#search_list::item::hover{background-color:rgb(206,207,"
                      "208);border:none;outline:none}");
  this->setStyleSheet("#search_list::focus{black;border:none;outline:none}");
}

void MainFrameSearchLists::slot_item_clicked(QListWidgetItem *clicked_item) {
  qDebug() << "item clicked! ";
}
