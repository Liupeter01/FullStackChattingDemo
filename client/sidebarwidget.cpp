#include "sidebarwidget.h"
#include <QStyleOption>
#include <QPainter>

SideBarWidget::SideBarWidget(QWidget *parent)
    :m_newMsg(new QLabel)
    , m_layout(new QVBoxLayout)
    , ClickableQLabel(parent)
{
}

SideBarWidget::~SideBarWidget()
{
}

//void SideBarWidget::paintEvent(QPaintEvent *)
//{
//    QStyleOption opt;
//    opt.initFrom(this);

//    QPainter p(this);
//    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//    return;
//}

void SideBarWidget::createMsgNoticeIns()
{
    this->m_newMsg->setObjectName("new_arriving_msg");
    this->m_newMsg->setAlignment(Qt::AlignCenter);

    this->m_layout->addWidget(m_newMsg);
    this->setLayout(m_layout);

    /*disable it at first*/
    this->setMsgNoticeVisibility();
}

void SideBarWidget::setMsgNoticeVisibility(bool status)
{
    m_newMsg->setVisible(status);
}

