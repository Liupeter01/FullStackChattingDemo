#include "forgotpassword.h"
#include <QMouseEvent>

ForgotPassword::ForgotPassword(QWidget *parent)
    : MultiClickableQLabel(parent) {
  this->setCursor(Qt::PointingHandCursor);
}

ForgotPassword::~ForgotPassword() {}

void ForgotPassword::mouseReleaseEvent(QMouseEvent *event)
{
    QLabel::mouseReleaseEvent(event);
}
