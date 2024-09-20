#ifndef CHATTINGHISTORYWIDGET_H
#define CHATTINGHISTORYWIDGET_H

#include "listitemwidgetbase.h"

namespace Ui {
class ChattingHistoryWidget;
}

class ChattingHistoryWidget : public ListItemWidgetBase {
  Q_OBJECT

public:
  ChattingHistoryWidget(QWidget *parent = nullptr);
  ~ChattingHistoryWidget();

  void setItemDisplay(const QString &_username, const QString &_avator,
                      const QString &_last_message);

private:
  Ui::ChattingHistoryWidget *ui;
  QString m_username;
  QString m_avatorpath;
  QString m_lastmsg;
};

#endif // CHATTINGHISTORYWIDGET_H
