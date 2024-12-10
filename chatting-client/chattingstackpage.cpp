#include <QUuid>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "chattingstackpage.h"
#include "chattingmsgitem.h"
#include "picturemsgbubble.h"
#include "textmsgbubble.h"
#include "ui_chattingstackpage.h"
#include <useraccountmanager.hpp>
#include <tcpnetworkconnection.h>

std::size_t ChattingStackPage::TXT_MSG_BUFFER_SIZE = 1024;

ChattingStackPage::ChattingStackPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChattingStackPage)
    , m_text_msg_counter(0)
{
  ui->setupUi(this);
}

ChattingStackPage::~ChattingStackPage() { delete ui; }

bool ChattingStackPage::isFriendCurrentlyChatting(const QString &target_uuid){
    return m_friendInfo->m_uuid == target_uuid;
}

void ChattingStackPage::setFriendInfo(std::shared_ptr<FriendChattingHistory> info){
    //m_friendInfo.reset();

    m_friendInfo = info->getUserNameCard();

    /*replace placeholder text*/
    ui->friend_name->setText(m_friendInfo->m_nickname);

    /*set chatting dlghistory for current friend conversation*/
    setChattingDlgHistory(info);
}

void ChattingStackPage::setChattingDlgHistory(std::shared_ptr<FriendChattingHistory> history)
{
    /*remove all items*/
    ui->chatting_record->removeAllItem();

    /*it could be msg, voice, video*/
    auto chat_data = history->getChattingHistory();

    std::visit([this](auto && T){
        if constexpr (std::is_same_v<ChattingTextMsg, std::decay_t<decltype(T)>>){
            parseChattingTextMsg(T);
        }
        else if constexpr (std::is_same_v<ChattingVoice, std::decay_t<decltype(T)>>){
            parseChattingVoice(T);
        }
        else if constexpr (std::is_same_v<ChattingVideo, std::decay_t<decltype(T)>>){
            parseChattingVideo(T);
        }
        else{
            qDebug() << "Chatting History DataType Error! Loading Failed!";
            return;
        }
    }, *chat_data);

    //ui->chatting_record->pushBackItem()
}

void ChattingStackPage::insertToHistoryList(std::shared_ptr<ChattingHistoryData> data, MsgType type){
    /*determine who is the msg sender*/
    QWidget *bubble{nullptr};
    ChattingMsgItem *item{nullptr};

    /*current user's uuid matching the msg sender's uuid*/
     ChattingRole role = ChattingRole::Sender;
    if(UserAccountManager::get_instance()->getCurUserInfo()->m_uuid == data->m_sender_uuid){
        role = ChattingRole::Sender;

        item = new ChattingMsgItem(role);
        item->setupUserName(UserAccountManager::get_instance()->getCurUserInfo()->m_nickname);
        item->setupIconPixmap(QPixmap(UserAccountManager::get_instance()->getCurUserInfo()->m_avatorPath));
    }
    else{
        auto friend_info = UserAccountManager::get_instance()->findAuthFriendsInfo(data->m_sender_uuid);
        if(!friend_info.has_value()){
            qDebug() << "User Friend Info Not Found!";
            return;
        }

        role = ChattingRole::Receiver;

        item = new ChattingMsgItem(role);
        item->setupUserName(friend_info.value()->m_nickname);
        item->setupIconPixmap(QPixmap(friend_info.value()->m_avatorPath));
    }

    if (type == MsgType::TEXT) {
        bubble = new TextMsgBubble(role, data->m_msg_content);
    } else if (type == MsgType::IMAGE) {
        bubble = new PictureMsgBubble(role, data->m_msg_content);
    } else if (type == MsgType::FILE) {
    }

    if (bubble != nullptr) {
        item->setupBubbleWidget(bubble);
        ui->chatting_record->pushBackItem(item);
    }
}

void ChattingStackPage::parseChattingTextMsg(const ChattingTextMsg &msg){
    for(auto &msg: msg.m_data){
        insertToHistoryList(msg, MsgType::TEXT);
    }
}

void ChattingStackPage::parseChattingVoice(const ChattingVoice &msg){
}

void ChattingStackPage::parseChattingVideo(const ChattingVideo &msg){
}

void ChattingStackPage::on_send_message_clicked() {
    QJsonObject obj;
    QJsonArray array;
    QString send_name = UserAccountManager::get_instance()->getCurUserInfo()->m_nickname;
    QString send_icon = UserAccountManager::get_instance()->getCurUserInfo()->m_avatorPath;

    const QVector<MsgInfo> &list = ui->user_input->getMsgList();

  for (std::size_t index = 0; index < list.size(); ++index) {
    /*currently, we are the msssage sender*/
      QWidget *bubble_send{nullptr};

    /*create this for send*/
      ChattingMsgItem *item_sender = new ChattingMsgItem(ChattingRole::Sender);

    MsgInfo info = list[index];

    item_sender->setupUserName(send_name);
    item_sender->setupIconPixmap(QPixmap(send_icon));

    /*msg sender and msg receiver identity*/
    obj["msg_sender"] = UserAccountManager::get_instance()->getCurUserInfo()->m_uuid;
    obj["msg_receiver"] = m_friendInfo->m_uuid;

    if (info.type == MsgType::TEXT) {
        bubble_send = new TextMsgBubble(ChattingRole::Sender, info.content);

        /*text msg buffer counter*/
        QJsonObject text_obj;
        if(m_text_msg_counter + info.content.length() > TXT_MSG_BUFFER_SIZE){
            text_obj["text_sender"] = obj["msg_sender"];
            text_obj["text_receiver"] = obj["msg_receiver"];
            text_obj["text_msg"] = array;

            QJsonDocument doc(obj);
            auto json = doc.toJson(QJsonDocument::Compact);

            /*clean all array value*/
            array = QJsonArray{};
            text_obj = QJsonObject{};

            /*clear this counter*/
            m_text_msg_counter = 0;

            SendNode<QByteArray, std::function<uint16_t(uint16_t)>> send_buffer(
                static_cast<uint16_t>(ServiceType::SERVICE_TEXTCHATMSGREQUEST), json,
                [](auto x) { return qToBigEndian(x); });

            /*after connection to server, send TCP request*/
            TCPNetworkConnection::get_instance()->send_data(std::move(send_buffer));
        }

        /*generate an unique uuid for this message*/
        obj["msg_id"] = QUuid::createUuid().toString();

        /*send message*/
        obj["msg_content"] = QString::fromUtf8(info.content.toUtf8());

        /*append current data to array*/
        array.append(obj);

        /*update counter*/
        m_text_msg_counter += info.content.length();

        /*
         * although the messages which are sent will appear on the chattingstackpage
         * the message will not be recorded by the in the chattinghistory which is
         * stored by UserAccountManager
         */
        emit signal_sync_chat_msg_on_local(
            info.type,
            std::make_shared<ChattingTextMsg>(
            obj["msg_sender"].toString(),
            obj["msg_receiver"].toString(),
            array
        ));

    } else if (info.type == MsgType::IMAGE) {
        bubble_send = new PictureMsgBubble(ChattingRole::Sender, info.pixmap);

    } else if (info.type == MsgType::FILE) {
    }

    if (bubble_send != nullptr) {
      item_sender->setupBubbleWidget(bubble_send);
      ui->chatting_record->pushBackItem(item_sender);
    }
  }

  /*if there is less data to send*/
  QJsonObject text_obj;
  text_obj["text_sender"] = obj["msg_sender"];
  text_obj["text_receiver"] = obj["msg_receiver"];
  text_obj["text_msg"] = array;
  QJsonDocument doc(obj);
  auto json = doc.toJson(QJsonDocument::Compact);

  /*clean all array value*/
  array = QJsonArray{};
  text_obj = QJsonObject{};

  /*clear this counter*/
  m_text_msg_counter = 0;

  SendNode<QByteArray, std::function<uint16_t(uint16_t)>> send_buffer(
      static_cast<uint16_t>(ServiceType::SERVICE_TEXTCHATMSGREQUEST), json,
      [](auto x) { return qToBigEndian(x); });

  /*after connection to server, send TCP request*/
  TCPNetworkConnection::get_instance()->send_data(std::move(send_buffer));
}
