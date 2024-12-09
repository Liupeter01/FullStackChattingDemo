#ifndef CHATTINGSTACKPAGE_H
#define CHATTINGSTACKPAGE_H

#include <QWidget>
#include <ChattingHistory.hpp>

namespace Ui {
class ChattingStackPage;
}

/*declaration*/
struct UserNameCard;
enum class MsgType;

class ChattingStackPage : public QWidget {
  Q_OBJECT

public:
  explicit ChattingStackPage(QWidget *parent = nullptr);
  virtual ~ChattingStackPage();

  void setFriendInfo(std::shared_ptr<FriendChattingHistory> info);

protected:
  void setChattingDlgHistory(std::shared_ptr<FriendChattingHistory> history);

  /*insert chatting history widget by push_back*/
  void insertToHistoryList(std::shared_ptr<ChattingHistoryData> data, MsgType type);

private:
  void parseChattingTextMsg(const ChattingTextMsg &msg);
  void parseChattingVoice(const ChattingVoice &msg);
  void parseChattingVideo(const ChattingVideo &msg);

signals:
  /*
   * expose chatting history data to main page
   * developers could update friend's request by using this signal
   */
  void signal_sync_chat_msg_on_local(MsgType msg_type, std::shared_ptr<ChattingTextMsg> msg);

private slots:
  void on_send_message_clicked();

private:
  Ui::ChattingStackPage *ui;

    std::size_t m_text_msg_counter;
  static std::size_t TXT_MSG_BUFFER_SIZE;

  /*target friend's info*/
  std::shared_ptr<UserNameCard> m_friendInfo;
};

#endif // CHATTINGSTACKPAGE_H
