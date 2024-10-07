#include "chattingrecordview.h"
#include <QListWidgetItem>
#include <QScrollBar>
#include <QStyleOption>

ChattingRecordView::ChattingRecordView(QWidget *parent)
    : m_newdataarrived(false), QListWidget{parent} {
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  /*install event filter*/
  this->viewport()->installEventFilter(this);

  /**/
  registerSignal();
}

ChattingRecordView::~ChattingRecordView() {}

void ChattingRecordView::slot_rangeChanged(int min, int max) {
  auto scroll_bar = this->verticalScrollBar();
  scroll_bar->setSliderPosition(scroll_bar->maximum());
}

void ChattingRecordView::registerSignal() {
  connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this,
          &ChattingRecordView::slot_rangeChanged);
}

void ChattingRecordView::pushBackItem(QWidget *item) {
  randomInsertItem(this->count(), item);
}

void ChattingRecordView::pushFrontItem(QWidget *item) {
  randomInsertItem(0, item);
}

void ChattingRecordView::randomInsertItem(int pos, QWidget *item) {
  /*invalid pos number*/
  if (pos < 0 || pos > this->count()) {
    return;
  }

  QListWidgetItem *inserted_item(new QListWidgetItem);

  // setup size according to widget size
  inserted_item->setSizeHint(item->sizeHint());

  this->insertItem(pos, inserted_item);
  this->setItemWidget(inserted_item, item);
  this->update();

  /*we need to add more content and extend scroll bar!*/
  m_newdataarrived = true;
}

bool ChattingRecordView::eventFilter(QObject *object, QEvent *event) {
  if (object == this->viewport()) {
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
      // qDebug() << "Load more chatting";
      // emit signal_load_more_record();
    }

    /**/
    // return true;
  }
  return QWidget::eventFilter(object, event);
}

void ChattingRecordView::paintEvent(QPaintEvent *event) {
  QWidget::paintEvent(event);
}
