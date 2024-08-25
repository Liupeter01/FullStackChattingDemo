#include "passworddisplayswitching.h"
#include <QDebug>
#include <QMouseEvent>

PasswordDisplaySwitching::PasswordDisplaySwitching(QWidget *parent,
                                                   Qt::WindowFlags f)
    : m_state(), QLabel(parent) {}

PasswordDisplaySwitching::~PasswordDisplaySwitching() {}

const LabelState &PasswordDisplaySwitching::getState() const { return m_state; }

void PasswordDisplaySwitching::mousePressEvent(QMouseEvent *event) {
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
void PasswordDisplaySwitching::enterEvent(QEnterEvent *event) {
  qDebug() << QString("mouse entered!");
  this->m_state.hover = LabelState::HoverStatus::ENABLED;
  Tools::setQLableImage(this, this->m_state.visiable
                                  ? "show_passwd_selected.png"
                                  : "invisiable_passwd_selected.png");
  QLabel::enterEvent(event);
}

/*mouse leave*/
void PasswordDisplaySwitching::leaveEvent(QEvent *event) {
  qDebug() << QString("mouse leave!");
  this->m_state.hover = LabelState::HoverStatus::DISABLED;

  Tools::setQLableImage(this, this->m_state.visiable
                                  ? "show_password.png"
                                  : "invisiable_password.png");

  QLabel::leaveEvent(event);
}
