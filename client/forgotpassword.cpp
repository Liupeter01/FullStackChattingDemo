#include "forgotpassword.h"
#include "tools.h"
#include <QDebug>
#include <QMouseEvent>

ForgotPassword::ForgotPassword(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent)
{
    this->setCursor(Qt::PointingHandCursor);
}

ForgotPassword::~ForgotPassword()
{

}

void ForgotPassword::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        qDebug() << QString("left button clicked!");
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}

void ForgotPassword::enterEvent(QEnterEvent *event)
{
    qDebug() << QString("mouse entered!");
    Tools::setWidgetAttribute(this, "forgot password?", true);
    QLabel::enterEvent(event);
}

void ForgotPassword::leaveEvent(QEvent *event)
{
    qDebug() << QString("mouse leave!");
    Tools::setWidgetAttribute(this, "forgot password?", false);
    QLabel::leaveEvent(event);
}
