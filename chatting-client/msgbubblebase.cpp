#include "msgbubblebase.h"
#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QRect>

MsgBubbleBase::MsgBubbleBase(ChattingRole role, QWidget *parent)
    : m_role(role), QFrame(parent), m_layout(new QHBoxLayout) {
  /*now we are the message sender*/
  if (role == ChattingRole::Sender) {
    /*                           |triangle_width|
     *---------------------------------------------------
     *                3                                 |
     *-----------------------------               |     |
     *|     |*********************|***************|     |
     *|<-3->|*******widget********|               |<-3->|
     *|     |*********************|               |     |
     *-----------------------------               |     |
     *                3                                 |
     *---------------------------------------------------
     */
    this->setContentsMargins(margin, margin, margin + triangle_width, margin);
  } else {
    /*      | triangle_width |
     * ---------------------------------------------------
     *                        3
     *                        ----------------------------
     *|     |****************|*********************|     |
     *|<-3->|                |*******widget********|<-3->|
     *|     |                |*********************|     |
     *                        ----------------------------
     *                        3
     * ---------------------------------------------------
     */
    this->setContentsMargins(margin + triangle_width, margin, margin, margin);
  }

  this->setLayout(m_layout);
}

MsgBubbleBase::~MsgBubbleBase() {}

void MsgBubbleBase::setupBubbleWidget(QWidget *widget) {
  /*there is nothing in the layout*/
  if (!m_layout->count()) {
    m_layout->addWidget(widget);
  }
}

void MsgBubbleBase::paintEvent(QPaintEvent *event) {
  QPainter painter(this);

  /*no outline*/
  painter.setPen(Qt::NoPen);

  /*now we are the message sender*/
  if (m_role == ChattingRole::Sender) {

    /*The similar color to green*/
    QColor sender_colour(0x9e, 0xea, 0x69);
    painter.setBrush(QBrush(sender_colour));

    /*draw a rectangular from()*/
    QRect sender_rect(0, 0, this->width() - triangle_width, this->height());

    /*draw a triangle*/
    QPointF points[3] = {
        QPointF(sender_rect.x() + sender_rect.width(), 12),
        QPointF(sender_rect.x() + sender_rect.width() + triangle_width,
                10 + triangle_width + triangle_width / 2),
        QPointF(sender_rect.x() + sender_rect.width(),
                12 + this->triangle_width + 2)};

    painter.drawRoundedRect(sender_rect, 5, 5);
    painter.drawPolygon(points, 3);
  } else {
    QColor receiver_colour(Qt::white);
    painter.setBrush(QBrush(receiver_colour));

    /*draw a rectangular*/
    QRect receiver_rect(triangle_width, 0, this->width() - triangle_width,
                        this->height());

    /*draw a triangle*/
    QPointF points[3] = {
        QPointF(receiver_rect.x(), 12),
        QPointF(receiver_rect.x() - triangle_width,
                10 + triangle_width + triangle_width / 2),
        QPointF(receiver_rect.x(), 12 + this->triangle_width + 2)};

    painter.drawRoundedRect(receiver_rect, 5, 5);
    painter.drawPolygon(points, 3);
  }
  return QFrame::paintEvent(event);
}
