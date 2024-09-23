#include "multiClickableQLabel.h"
#include <QDebug>
#include <QMouseEvent>

MultiClickableQLabel::MultiClickableQLabel(QWidget *parent, Qt::WindowFlags f)
    : m_state(), QLabel(parent) {}

MultiClickableQLabel::~MultiClickableQLabel() {}

const LabelState &MultiClickableQLabel::getState() const { return m_state; }

void MultiClickableQLabel::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    qDebug() << QString("left button release!");

    /*switch visiable status after each mouse press event*/
    this->m_state.visiable =
        (!this->m_state.visiable ? LabelState::VisiableStatus::ENABLED
                                 : LabelState::VisiableStatus::DISABLED);

    emit clicked();
  }
  QLabel::mouseReleaseEvent(event);
}

void MultiClickableQLabel::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    qDebug() << QString("left button clicked!");

    /*switch visiable status after each mouse press event*/
    this->m_state.visiable =
        (!this->m_state.visiable ? LabelState::VisiableStatus::ENABLED
                                 : LabelState::VisiableStatus::DISABLED);

    emit clicked();
  }
  QLabel::mousePressEvent(event);
}

/*mouse enter selected section*/
void MultiClickableQLabel::enterEvent(QEnterEvent *event) {
  qDebug() << QString("mouse entered!");
  this->m_state.hover = LabelState::HoverStatus::ENABLED;
  emit update_display();
  QLabel::enterEvent(event);
}

/*mouse leave*/
void MultiClickableQLabel::leaveEvent(QEvent *event) {
  qDebug() << QString("mouse leave!");
  this->m_state.hover = LabelState::HoverStatus::DISABLED;
  emit update_display();
  QLabel::leaveEvent(event);
}
