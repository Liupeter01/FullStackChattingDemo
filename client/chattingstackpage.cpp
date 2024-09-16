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
  //ChattingRole role = ChattingRole::Sender;
  QString recv_name = "Bob";
  QString send_name = "Alex";
  QString recv_icon = QT_DEMO_HOME "/res/recv.png";
  QString send_icon = QT_DEMO_HOME "/res/send.png";

  const QVector<MsgInfo> &list = ui->user_input->getMsgList();



  for (std::size_t index = 0; index < list.size(); ++index) {
    /*currently, we are the msssage sender*/
    QWidget *bubble_send{nullptr}, *bubble_recv{nullptr};

    /*create this for send*/
    ChattingMsgItem *item_sender = new ChattingMsgItem(ChattingRole::Sender);

    /*create this for recv*/
    ChattingMsgItem *item_recv = new ChattingMsgItem(ChattingRole::Receiver);

    MsgInfo info = list[index];

    item_sender->setupUserName(send_name);
    item_sender->setupIconPixmap(QPixmap(send_icon));

    item_recv->setupUserName(recv_name);
    item_recv->setupIconPixmap(QPixmap(recv_icon));

    if (info.type == MsgType::TEXT) {
      bubble_send = new TextMsgBubble(ChattingRole::Sender, info.content);
    } else if (info.type == MsgType::IMAGE) {
      bubble_send = new PictureMsgBubble(ChattingRole::Sender, info.pixmap);
    } else if (info.type == MsgType::FILE) {
    }

    bubble_recv = new TextMsgBubble(ChattingRole::Receiver, QString("recv success!"));

    if (bubble_send != nullptr && bubble_recv != nullptr) {
      item_sender->setupBubbleWidget(bubble_send);
      item_recv->setupBubbleWidget(bubble_recv);
      ui->chatting_record->pushBackItem(item_sender);
      ui->chatting_record->pushBackItem(item_recv);
    }
  }
}
