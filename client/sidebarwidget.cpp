#include "sidebarwidget.h"
#include <QPainter>
#include <QStyleOption>

SideBarWidget::SideBarWidget(QWidget *parent)
    : m_newMsg(new QLabel), m_layout(new QVBoxLayout), ClickableQLabel(parent) {
}

SideBarWidget::~SideBarWidget() {}

void SideBarWidget::clearState()
{
    m_state.hover = LabelState::HoverStatus::DISABLED;
    m_state.visiable = LabelState::VisiableStatus::DISABLED;
    emit update_display();
}

void SideBarWidget::mousePressEvent(QMouseEvent *event)
{
  QLabel::mousePressEvent(event);
}

void SideBarWidget::createMsgNoticeIns() {
  this->m_newMsg->setObjectName("new_arriving_msg");
  this->m_newMsg->setAlignment(Qt::AlignCenter);

  this->m_layout->addWidget(m_newMsg);
  this->setLayout(m_layout);

  /*disable it at first*/
  this->setMsgNoticeVisibility();
}

void SideBarWidget::setMsgNoticeVisibility(bool status) {
  m_newMsg->setVisible(status);
}
