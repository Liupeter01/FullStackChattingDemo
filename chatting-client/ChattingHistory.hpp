#ifndef CHATTINGHISTORY_HPP
#define CHATTINGHISTORY_HPP
#include <vector>
#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <UserNameCard.h>

struct ChattingHistoryData{
    ChattingHistoryData(const QString &sender,
                        const QString &receiver,
                        const QString &id,
                        const QString &msg);

    QString m_sender_uuid;
    QString m_receiver_uuid;
    QString m_msg_id;
    QString m_msg_content;
};

struct ChattingTextMsg{
    ChattingTextMsg(const QString &sender,
                    const QString &receiver,
                    const QJsonArray & msg_data);

    QString sender_uuid;
    QString receiver_uuid;
    std::vector<std::shared_ptr<ChattingHistoryData>> m_data;
};

struct ChattingVoice{};
struct ChattingVideo{};

/*store the friend's identity and the historical info sent before*/
struct FriendChattingHistory: public UserNameCard{
    using ChatType = std::variant<ChattingTextMsg, ChattingVoice, ChattingVideo>;

    FriendChattingHistory(const QString &uuid, const QString &avator_path,
                          const QString &username, const QString &nickname,
                          const QString &desc, Sex sex, const ChatType &type) noexcept;

    template<typename _Type, typename It, typename std::enable_if<
                                              std::is_same_v<ChattingTextMsg, std::decay_t<_Type>>
                                                  || std::is_same_v<ChattingVoice, std::decay_t<_Type>>
                                                  || std::is_same_v<ChattingVideo, std::decay_t<_Type>>, int>::type = 0>
    void updateChattingHistory(It begin, It end){
        auto &target = std::get<_Type>(*getChattingHistory());
        std::copy(begin, end, std::back_inserter(target.m_data));
    }

    std::shared_ptr<UserNameCard> getUserNameCard();
    std::shared_ptr<ChatType> getChattingHistory();

protected:
    std::shared_ptr<ChatType> m_chatData;
};

#endif // CHATTINGHISTORY_HPP
