#include "chattingstackpage.h"
#include "chattingmsgitem.h"
#include "picturemsgbubble.h"
#include "textmsgbubble.h"
#include "ui_chattingstackpage.h"

ChattingStackPage::ChattingStackPage(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChattingStackPage) {
  ui->setupUi(this);
}

ChattingStackPage::~ChattingStackPage() { delete ui; }

void ChattingStackPage::on_send_message_clicked() {
  ChattingRole role = ChattingRole::Sender;
  QString username = "test_username";
  QString iconpath = QT_DEMO_HOME "/res/header.png";

  const QVector<MsgInfo> &list = ui->user_input->getMsgList();

  for (std::size_t index = 0; index < list.size(); ++index) {
    /*currently, we are the msssage sender*/
    QWidget *bubble{nullptr};
    ChattingMsgItem *item = new ChattingMsgItem(role);
    MsgInfo info = list[index];

    item->setupUserName(username);
    item->setupIconPixmap(QPixmap(iconpath));

    if (info.type == MsgType::TEXT) {
      bubble = new TextMsgBubble(role, info.content);
    } else if (info.type == MsgType::IMAGE) {
      bubble = new PictureMsgBubble(role, info.pixmap);
    } else if (info.type == MsgType::FILE) {
    }

    if (bubble != nullptr) {
      item->setupBubbleWidget(bubble);
      ui->chatting_record->pushBackItem(item);
    }
  }
}
