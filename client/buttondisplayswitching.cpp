#include "buttondisplayswitching.h"
#include <QMouseEvent>

ButtonDisplaySwitching::ButtonDisplaySwitching(QWidget *parent)
    : m_state(), QPushButton(parent) {}

ButtonDisplaySwitching::~ButtonDisplaySwitching() {}

const PushButtonState &ButtonDisplaySwitching::getState() const {
  return m_state;
}

void ButtonDisplaySwitching::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    qDebug() << QString("left button clicked!");

    /*switch visiable status after each mouse press event*/
    this->m_state.select =
        (!this->m_state.select ? PushButtonState::SelectedStatus::ENABLED
                               : PushButtonState::SelectedStatus::DISABLED);

    emit clicked();
  }
  QPushButton::mousePressEvent(event);
}

void ButtonDisplaySwitching::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    qDebug() << QString("left button released!");

    /*switch visiable status after each mouse press event*/
    this->m_state.select =
        (!this->m_state.select ? PushButtonState::SelectedStatus::ENABLED
                               : PushButtonState::SelectedStatus::DISABLED);

    emit clicked();
  }
  QPushButton::mouseReleaseEvent(event);
}

void ButtonDisplaySwitching::enterEvent(QEnterEvent *event) {
  qDebug() << QString("mouse entered!");
  this->m_state.hover = PushButtonState::HoverStatus::ENABLED;
  emit update_display();
  QPushButton::enterEvent(event);
}

void ButtonDisplaySwitching::leaveEvent(QEvent *event) {
  qDebug() << QString("mouse leave!");
  this->m_state.hover = PushButtonState::HoverStatus::DISABLED;
  emit update_display();
  QPushButton::leaveEvent(event);
}
