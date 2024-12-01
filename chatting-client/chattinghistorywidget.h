#ifndef CHATTINGHISTORYWIDGET_H
#define CHATTINGHISTORYWIDGET_H

#include <memory>
#include <UserNameCard.h>
#include "listitemwidgetbase.h"

namespace Ui {
class ChattingHistoryWidget;
}

class ChattingHistoryWidget : public ListItemWidgetBase {
  Q_OBJECT

public:
  ChattingHistoryWidget(QWidget *parent = nullptr);
  virtual ~ChattingHistoryWidget();

  void setUserInfo(std::shared_ptr<UserNameCard> info);
  void setLastMsg(const QString& lastmsg);
  void setItemDisplay();

private:
  Ui::ChattingHistoryWidget *ui;
  std::shared_ptr<UserNameCard> m_userinfo;
  QString m_lastmsg;
};

#endif // CHATTINGHISTORYWIDGET_H
