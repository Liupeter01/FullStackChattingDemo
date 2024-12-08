#include <ChattingHistory.hpp>

ChattingHistoryData::ChattingHistoryData(const QString &sender,
                                         const QString &receiver,
                                         const QString &id,
                                         const QString &msg)
    : m_sender_uuid(sender)
    , m_receiver_uuid(receiver)
    , m_msg_id(id)
    , m_msg_content(msg)
{}

ChattingTextMsg::ChattingTextMsg(const QString &sender,
                                 const QString &receiver,
                                 const QJsonArray &msg_data)
    : sender_uuid(sender)
    , receiver_uuid(receiver)
{
    for(const auto &msg: msg_data){
        auto obj = msg.toObject();
        m_data.push_back(std::make_shared<ChattingHistoryData>(
            obj["msg_sender"].toString(),
            obj["msg_receiver"].toString(),
            obj["msg_id"].toString(),
            obj["msg_content"].toString()
            ));
    }
}

FriendChattingHistory::FriendChattingHistory(const QString &uuid,
                                             const QString &avator_path,
                                             const QString &username,
                                             const QString &nickname,
                                             const QString &desc,
                                             Sex sex,
                                             const ChatType &type) noexcept
    :UserNameCard(uuid, avator_path, username, nickname, desc, sex)
{
    std::visit([this](auto &&T){
        if constexpr (std::is_same_v<ChattingTextMsg, std::decay_t<decltype(T)>>
                      || std::is_same_v<ChattingVoice, std::decay_t<decltype(T)>>
                      || std::is_same_v<ChattingVideo, std::decay_t<decltype(T)>>){
            m_chatData = std::make_shared<ChatType>(T);
        }
        else{
            qDebug() << "Chatting History DataType Error!";
        }
    }, type);
}

std::shared_ptr<UserNameCard> FriendChattingHistory::getUserNameCard(){
    return std::make_shared<UserNameCard>(*this);
}

std::shared_ptr<FriendChattingHistory::ChatType> FriendChattingHistory::getChattingHistory(){
    return this->m_chatData;
}
