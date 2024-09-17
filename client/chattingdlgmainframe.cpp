#include "chattingdlgmainframe.h"
#include "loadingwaitdialog.h"
#include "tools.h"
#include "ui_chattingdlgmainframe.h"
#include <QAction>
#include <QFile>
#include <QRandomGenerator>
#include <listitemwidget.h>

ChattingDlgMainFrame::ChattingDlgMainFrame(QWidget *parent)
    : QDialog(parent), ui(new Ui::ChattingDlgMainFrame),
      m_dlgMode(
          ChattingDlgMode::ChattingDlgChattingMode) /*chatting mode by default*/
{
  ui->setupUi(this);

  /*register signal for ui display*/
  registerSignal();

  /*register Qaction for search edit ui widget*/
  registerSearchEditAction();

  /*register search edit signal*/
  registerSearchEditSignal();

  /*constraint the length of username when client try to search*/
  ui->search_user_edit->setMaxLength(20);

  /*set show list to hidden status*/
  // ui->show_lists->setHidden(true);
  addItemToShowLists();

  /*load qicon for chatting main frame*/
  Tools::loadIconResources({"add_friend_normal.png", "add_friend_hover.png",
                            "add_friend_clicked.png"});

  /*set default button icon*/
  Tools::setPushButtonIcon(ui->search_user_button, "add_friend_normal.png");

  /*load qimage for side bar*/
  Tools::loadImgResources({"chat_icon_normal.png", "chat_icon_hover.png",
                           "chat_icon_clicked.png", "contact_list_normal.png",
                           "contact_list_hover.png",
                           "contact_list_clicked.png"},
                          (ui->my_chat->width() + ui->my_chat->width()) / 2,
                          (ui->my_chat->height() + ui->my_chat->height()) / 2);

  Tools::setQLableImage(ui->my_chat, "chat_icon_normal.png");
  Tools::setQLableImage(ui->my_contact, "contact_list_normal.png");
}

void ChattingDlgMainFrame::registerSignal() {
  connect(ui->search_user_button, &ButtonDisplaySwitching::clicked, this,
          &ChattingDlgMainFrame::updateSearchUserButton);
  connect(ui->search_user_button, &ButtonDisplaySwitching::update_display, this,
          &ChattingDlgMainFrame::updateSearchUserButton);
  connect(ui->my_chat, &SideBarWidget::clicked, this,
          &ChattingDlgMainFrame::updateMyChat);
  connect(ui->my_chat, &SideBarWidget::update_display, this,
          &ChattingDlgMainFrame::updateMyChat);

  connect(ui->my_contact, &SideBarWidget::clicked, this,
          &ChattingDlgMainFrame::updateMyContact);
  connect(ui->my_contact, &SideBarWidget::update_display, this,
          &ChattingDlgMainFrame::updateMyContact);

  connect(ui->show_lists, &MainFrameShowLists::signal_load_more_record, this,
          &ChattingDlgMainFrame::slot_load_more_record);
}

void ChattingDlgMainFrame::registerSearchEditAction() {
  /*add a search icon*/
  m_searchAction = new QAction(ui->search_user_edit);
  m_searchAction->setIcon(
      Tools::loadIcon(QT_DEMO_HOME "/res/search.png").value());

  /*put it on the front position of line edit*/
  ui->search_user_edit->addAction(m_searchAction, QLineEdit::LeadingPosition);
  ui->search_user_edit->setPlaceholderText(QString("Searching"));

  /*add a transparent cancel button*/
  m_cancelAction = new QAction(ui->search_user_edit);
  m_cancelAction->setIcon(
      Tools::loadIcon(QT_DEMO_HOME "/res/close_transparent.png").value());

  /*put it on the back position of line edit*/
  ui->search_user_edit->addAction(m_cancelAction, QLineEdit::TrailingPosition);

  // connect(ui->search_user_edit, )
}

void ChattingDlgMainFrame::registerSearchEditSignal() {
  /*when user input sth, then change transparent icon to visible icon*/
  connect(ui->search_user_edit, &QLineEdit::textChanged, this,
          [this](const QString &str) {
            m_cancelAction->setIcon(
                Tools::loadIcon(str.isEmpty()
                                    ? QT_DEMO_HOME "/res/close_transparent.png"
                                    : QT_DEMO_HOME "/res/close_search.png")
                    .value());
          });

  /*when user trigger cancel button, then clear all the text*/
  connect(m_cancelAction, &QAction::triggered, [this]() {
    /*clear username search text*/
    ui->search_user_edit->clear();

    /*switch to transparent icon, because there is no input*/
    m_cancelAction->setIcon(
        Tools::loadIcon(QT_DEMO_HOME "/res/close_transparent.png").value());

    /**/
    ui->search_user_edit->clearFocus();

    /*set show list to hidden status*/
    ui->show_lists->setHidden(true);
  });
}

void ChattingDlgMainFrame::updateSearchUserButton() {
  auto state = ui->search_user_button->getState();
  /*if it is selected, then it gets the highest proity*/
  if (state.select == PushButtonState::SelectedStatus::ENABLED) {
    setCursor(Qt::PointingHandCursor);
    Tools::setPushButtonIcon(ui->search_user_button, "add_friend_clicked.png");
  } else {
    /*currently, its not selected! switch to hover
     *if it is not hovered! then switch to normal
     */
    Tools::setPushButtonIcon(ui->search_user_button,

                             state.hover ==
                                     PushButtonState::HoverStatus::DISABLED
                                 ? "add_friend_normal.png"
                                 : "add_friend_hover.png");

    if (state.hover == PushButtonState::HoverStatus::ENABLED) {
      setCursor(Qt::PointingHandCursor);
    } else {
      unsetCursor();
    }
  }
}

void ChattingDlgMainFrame::updateMyChat() {
  auto state = ui->my_chat->getState();
  if (state.visiable == LabelState::VisiableStatus::ENABLED) {
    setCursor(Qt::PointingHandCursor);
    Tools::setQLableImage(ui->my_chat, "chat_icon_clicked.png");
  } else {
    Tools::setQLableImage(ui->my_chat,
                          state.hover == LabelState::HoverStatus::DISABLED
                              ? "chat_icon_normal.png"
                              : "chat_icon_hover.png");

    if (state.hover == LabelState::HoverStatus::ENABLED) {
      setCursor(Qt::PointingHandCursor);
    } else {
      unsetCursor();
    }
  }
}

void ChattingDlgMainFrame::updateMyContact() {
  auto state = ui->my_contact->getState();
  if (state.visiable == LabelState::VisiableStatus::ENABLED) {
    setCursor(Qt::PointingHandCursor);
    Tools::setQLableImage(ui->my_contact, "contact_list_clicked.png");
  } else {
    Tools::setQLableImage(ui->my_contact,
                          state.hover == LabelState::HoverStatus::DISABLED
                              ? "contact_list_normal.png"
                              : "contact_list_hover.png");

    if (state.hover == LabelState::HoverStatus::ENABLED) {
      setCursor(Qt::PointingHandCursor);
    } else {
      unsetCursor();
    }
  }
}

void ChattingDlgMainFrame::slot_load_more_record() {
  LoadingWaitDialog *loadingInf(new LoadingWaitDialog(this));

  /*do not block the execute flow*/
  loadingInf->setModal(true);
  loadingInf->show();

  /*load more data to the list*/
  qDebug() << "load more data to the list";
  addItemToShowLists();

  loadingInf->deleteLater();
}

ChattingDlgMainFrame::~ChattingDlgMainFrame() {
  delete m_searchAction;
  delete m_cancelAction;
  delete ui;
}

void ChattingDlgMainFrame::addItemToShowLists() {
  for (std::size_t i = 0; i < 40; ++i) {
    auto random = QRandomGenerator::global()->bounded(10000);
    ListItemWidget *new_inserted(new ListItemWidget());
    new_inserted->setItemDisplay(QString::number(random),
                                 QT_DEMO_HOME "/res/microsoft.png",
                                 QString::number(random));

    QListWidgetItem *item(new QListWidgetItem);
    item->setSizeHint(new_inserted->sizeHint());

    ui->show_lists->addItem(item);
    ui->show_lists->setItemWidget(item, new_inserted);
    ui->show_lists->update();
  }
}
