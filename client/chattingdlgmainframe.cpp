#include "chattingdlgmainframe.h"
#include "ui_chattingdlgmainframe.h"
#include <QFile>
#include "tools.h"

ChattingDlgMainFrame::ChattingDlgMainFrame(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChattingDlgMainFrame)
{
    ui->setupUi(this);

    /*register signal for ui display*/
    registerSignal();

    /*load qicon for chatting main frame*/
    Tools::loadIconResources({
        "add_friend_normal.png",
        "add_friend_hover.png",
        "add_friend_clicked.png"
    });

    /*set default button icon*/
    Tools::setPushButtonIcon(ui->search_user_button,
                             "add_friend_normal.png");
}

void ChattingDlgMainFrame::registerSignal() {
    connect(ui->search_user_button,
            &ButtonDisplaySwitching::clicked, this,
            &ChattingDlgMainFrame::updateSearchUserButton
            );
    connect(ui->search_user_button,
            &ButtonDisplaySwitching::update_display, this,
            &ChattingDlgMainFrame::updateSearchUserButton
            );
}

void ChattingDlgMainFrame::updateSearchUserButton()
{
    auto state = ui->search_user_button->getState();
    /*if it is selected, then it gets the highest proity*/
    if (state.select == PushButtonState::SelectedStatus::ENABLED) {
        setCursor(Qt::PointingHandCursor);
        Tools::setPushButtonIcon(
            ui->search_user_button,
            "add_friend_clicked.png"
            );
    }
    else
    {
        /*currently, its not selected! switch to hover
         *if it is not hovered! then switch to normal
         */
        Tools::setPushButtonIcon(
            ui->search_user_button,

            state.hover == PushButtonState::HoverStatus::DISABLED ?
                "add_friend_normal.png" : "add_friend_hover.png"
            );

        if(state.hover == PushButtonState::HoverStatus::ENABLED){
            setCursor(Qt::PointingHandCursor);
        }
        else{
            unsetCursor();
        }

    }
}

ChattingDlgMainFrame::~ChattingDlgMainFrame()
{
    delete ui;
}
