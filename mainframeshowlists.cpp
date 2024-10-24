#include "mainframeshowlists.h"
#include <QScrollBar>
#include <QWheelEvent>

MainFrameShowLists::MainFrameShowLists(QWidget *parent) : QListWidget(parent) {
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  /*install event filter to support customlized functions*/
  this->viewport()->installEventFilter(this);
}

MainFrameShowLists::~MainFrameShowLists() {}

bool MainFrameShowLists::eventFilter(QObject *object, QEvent *event) {
  /*check mouse's status: hover&entered or leave*/
  if (this->viewport() == object) {
    if (event->type() == QEvent::Enter) {
      /*mouse hover&enter then show the scroll bar*/
      this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    } else if (event->type() == QEvent::Leave) {
      /*mouse leave then hide the scroll bar*/
      this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
  }

  /*check mouse wheel event*/
  if (this->viewport() == object && event->type() == QEvent::Wheel) {
    QWheelEvent *wheel = reinterpret_cast<QWheelEvent *>(event);

    /*calculate the up/down parameters*/
    int degrees = wheel->angleDelta().y() / 8;
    int steps = degrees / 15;

    this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() -
                                        steps);

    /* the scrollbar has already arrived at the button
     * check should we load more chatting user*/
    QScrollBar *scroll = this->verticalScrollBar();
    if (scroll->maximum() - scroll->value() <= 0) {
      qDebug() << "Load more chatting";
      emit signal_load_more_record();
    }

    /**/
    // return true;
  }

  return QListWidget::eventFilter(object, event);
}
