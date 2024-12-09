#ifndef CHATTINGHISTORYWIDGET_H
#define CHATTINGHISTORYWIDGET_H

#include "ChattingHistory.hpp"
#include "listitemwidgetbase.h"

namespace Ui {
class ChattingHistoryWidget;
}

enum class MsgType;
struct FriendChattingHistory;

struct ChattingTextMsg;
struct ChattingVoice;
struct ChattingVideo;

class ChattingHistoryWidget : public ListItemWidgetBase {
  Q_OBJECT

public:
  ChattingHistoryWidget(QWidget *parent = nullptr);
  virtual ~ChattingHistoryWidget();

  void setUserInfo(std::shared_ptr<FriendChattingHistory> info);
  void setLastMsg(const QString &lastmsg);
  void setItemDisplay();

  std::shared_ptr<FriendChattingHistory> getChattingContext();

private:
  Ui::ChattingHistoryWidget *ui;
  std::shared_ptr<FriendChattingHistory> m_userinfo;
};

#endif // CHATTINGHISTORYWIDGET_H
