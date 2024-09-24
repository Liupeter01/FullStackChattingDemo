#include "onceclickableqlabel.h"
#include <QDebug>
#include <QMouseEvent>

OnceClickableQLabel::OnceClickableQLabel(QWidget *parent) {
  this->setCursor(Qt::PointingHandCursor);
}

OnceClickableQLabel::~OnceClickableQLabel() {}

void OnceClickableQLabel::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    qDebug() << QString("left button clicked!");
    emit clicked(this->text());
    // return;
  }
  QLabel::mousePressEvent(event);
}
