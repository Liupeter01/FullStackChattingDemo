#include "onceclickableqlabel.h"
#include <QDebug>
#include <QMouseEvent>

OnceClickableQLabel::OnceClickableQLabel(QWidget *parent) {
  this->setCursor(Qt::PointingHandCursor);
}

OnceClickableQLabel::~OnceClickableQLabel() {}

void OnceClickableQLabel::setCurrentState(LabelState::VisiableStatus status) {
  m_state.visiable = status;
  emit update_display(this->text(), m_state);
}

void OnceClickableQLabel::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    qDebug() << QString("left button clicked!");

    /*switch visiable status after each mouse press event*/
    this->m_state.visiable =
        (!this->m_state.visiable ? LabelState::VisiableStatus::ENABLED
                                 : LabelState::VisiableStatus::DISABLED);

    emit clicked(this->text(), m_state);
  }
  QLabel::mousePressEvent(event);
}
