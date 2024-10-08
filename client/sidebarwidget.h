#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include "multiclickableqlabel.h"
#include <QVBoxLayout>

class SideBarWidget : public MultiClickableQLabel {
  Q_OBJECT

public:
  explicit SideBarWidget(QWidget *parent = nullptr);
  virtual ~SideBarWidget();

public:
  /*clear LabelState*/
  void clearState();

protected:
  virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
  /*create a red point to indicate new msg/invite arrived*/
  void createMsgNoticeIns();
  void setMsgNoticeVisibility(bool status = false);

private:
  QLabel *m_newMsg;
  QVBoxLayout *m_layout;
};

#endif // SIDEBARWIDGET_H
