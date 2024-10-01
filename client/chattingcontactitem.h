#ifndef CHATTINGCONTACTITEM_H
#define CHATTINGCONTACTITEM_H

#include "listitemwidgetbase.h"
#include <QString>
#include <QWidget>

namespace Ui {
class ChattingContactItem;
}

class ChattingContactItem : public ListItemWidgetBase {
  Q_OBJECT

public:
  explicit ChattingContactItem(QWidget *parent = nullptr);
  ~ChattingContactItem();

public:
  virtual QSize sizeHint() const;

  /*set AddUserWidget*/
  void setAddUserWidget();

  /*set chatting contact info*/
  void setChattingContact(const QString &target_picture, const QString &text);

  /*set dialog with seperator*/
  void setGroupSeperator(const QString &text);

private:
  QSize m_size;
  Ui::ChattingContactItem *ui;
  const QString static_text;
};

#endif // CHATTINGCONTACTITEM_H
