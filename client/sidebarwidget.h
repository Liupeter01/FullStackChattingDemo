#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QVBoxLayout>
#include "clickableqlabel.h"

class SideBarWidget : public ClickableQLabel
{
    Q_OBJECT

public:
    explicit SideBarWidget(QWidget* parent = nullptr);
    ~SideBarWidget();

protected:
    //void paintEvent(QPaintEvent *) override;

private:
    /*create a red point to indicate new msg/invite arrived*/
    void createMsgNoticeIns();
    void setMsgNoticeVisibility(bool status = false);

private:
    QLabel *m_newMsg;
    QVBoxLayout *m_layout;
};

#endif // SIDEBARWIDGET_H
