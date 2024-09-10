#include "chattingdlgmainframe.h"
#include "tools.h"
#include "ui_chattingdlgmainframe.h"
#include <QAction>
#include <QFile>

ChattingDlgMainFrame::ChattingDlgMainFrame(QWidget *parent)
    : QDialog(parent), ui(new Ui::ChattingDlgMainFrame) {
  ui->setupUi(this);

  /*register signal for ui display*/
  registerSignal();

  /*register Qaction for search edit ui widget*/
  registerSearchEditAction();

  /*register search edit signal*/
  registerSearchEditSignal();

  /*constraint the length of username when client try to search*/
  ui->search_user_edit->setMaxLength(20);

  /*load qicon for chatting main frame*/
  Tools::loadIconResources({"add_friend_normal.png", "add_friend_hover.png",
                            "add_friend_clicked.png"});

  /*set default button icon*/
  Tools::setPushButtonIcon(ui->search_user_button, "add_friend_normal.png");
}

void ChattingDlgMainFrame::registerSignal() {
  connect(ui->search_user_button, &ButtonDisplaySwitching::clicked, this,
          &ChattingDlgMainFrame::updateSearchUserButton);
  connect(ui->search_user_button, &ButtonDisplaySwitching::update_display, this,
          &ChattingDlgMainFrame::updateSearchUserButton);
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

ChattingDlgMainFrame::~ChattingDlgMainFrame() {
  delete m_searchAction;
  delete m_cancelAction;
  delete ui;
}
