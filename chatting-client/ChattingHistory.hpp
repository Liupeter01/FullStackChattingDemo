#ifndef CHATTINGHISTORY_HPP
#define CHATTINGHISTORY_HPP
#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <UserNameCard.h>
#include <useraccountmanager.hpp>
#include <vector>

struct ChattingHistoryData {
  ChattingHistoryData(const QString &sender, const QString &receiver,
                      const QString &id, const QString &msg);

  QString m_sender_uuid;
  QString m_receiver_uuid;
  QString m_msg_id;
  QString m_msg_content;
};

struct ChattingTextMsg {
  ChattingTextMsg(const QString &sender, const QString &receiver);
  ChattingTextMsg(const QString &sender, const QString &receiver,
                  const QJsonArray &msg_data);

  QString sender_uuid;
  QString receiver_uuid;
  std::vector<std::shared_ptr<ChattingHistoryData>> m_data;
};

struct ChattingVoice {
  ChattingVoice(const QString &sender, const QString &receiver);

  QString sender_uuid;
  QString receiver_uuid;
};
struct ChattingVideo {
  ChattingVideo(const QString &sender, const QString &receiver);

  QString sender_uuid;
  QString receiver_uuid;
};

template <typename _Type>
using check_datatype_v = typename std::enable_if<
    std::is_same_v<ChattingTextMsg, std::decay_t<_Type>> ||
        std::is_same_v<ChattingVoice, std::decay_t<_Type>> ||
        std::is_same_v<ChattingVideo, std::decay_t<_Type>>,
    int>::type;

/*store the friend's identity and the historical info sent before*/
struct FriendChattingHistory : public UserNameCard {
  using ChatType = std::variant<ChattingTextMsg, ChattingVoice, ChattingVideo>;

  template <typename _Type, check_datatype_v<_Type> = 0>
  inline FriendChattingHistory(UserNameCard &&card) noexcept
      : FriendChattingHistory(
            card,
            _Type{
                card.m_uuid,
                UserAccountManager::get_instance()->getCurUserInfo()->m_uuid}) {
  }

  template <typename _Type, check_datatype_v<_Type> = 0>
  inline FriendChattingHistory(std::shared_ptr<UserNameCard> &card) noexcept
      : FriendChattingHistory(
            card,
            _Type{
                card->m_uuid,
                UserAccountManager::get_instance()->getCurUserInfo()->m_uuid}) {
  }

  template <typename _Type, check_datatype_v<_Type> = 0>
  inline FriendChattingHistory(std::shared_ptr<UserNameCard> card) noexcept
      : FriendChattingHistory(
            card,
            _Type{
                card->m_uuid,
                UserAccountManager::get_instance()->getCurUserInfo()->m_uuid}) {
  }

  template <typename _Type, check_datatype_v<_Type> = 0>
  inline FriendChattingHistory(const UserNameCard &card) noexcept
      : FriendChattingHistory(
            card,
            _Type{
                card.m_uuid,
                UserAccountManager::get_instance()->getCurUserInfo()->m_uuid}) {
  }

  FriendChattingHistory(std::shared_ptr<UserNameCard> card,
                        const ChatType &type) noexcept;

  FriendChattingHistory(const UserNameCard &card,
                        const ChatType &type) noexcept;

  FriendChattingHistory(const QString &uuid, const QString &avator_path,
                        const QString &username, const QString &nickname,
                        const QString &desc, Sex sex,
                        const ChatType &type) noexcept;

  template <typename _Type, typename It, check_datatype_v<_Type> = 0>
  void updateChattingHistory(It begin, It end) {
    auto &target = std::get<_Type>(*getChattingHistory());
    std::copy(begin, end, std::back_inserter(target.m_data));
  }

  std::shared_ptr<UserNameCard> getUserNameCard();
  std::shared_ptr<ChatType> getChattingHistory();

protected:
  std::shared_ptr<ChatType> m_chatData;
};

#endif // CHATTINGHISTORY_HPP
