#include "chattingrecordview.h"
#include <QScrollBar>
#include <QHBoxLayout>

ChattingRecordView::ChattingRecordView(QWidget *parent)
    : m_newdataarrived(false)
    , QListWidget{parent}
    , m_layout(new QVBoxLayout)
    , m_scroll(new QScrollArea)
{
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    /*install event filter*/
    this->viewport()->installEventFilter(this);

    /**/
    registerSignal();
}

void ChattingRecordView::slot_rangeChanged(int min, int max)
{
    auto scroll_bar =this->verticalScrollBar();
    scroll_bar->setSliderPosition(scroll_bar->maximum());
}

void ChattingRecordView::registerSignal()
{
    connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this, &ChattingRecordView::slot_rangeChanged);
}

void ChattingRecordView::pushBackItem(QWidget *item)
{
}

void ChattingRecordView::pushFrontItem(QWidget *item)
{
}

void ChattingRecordView::randomInsertItem(QWidget *pos, QWidget *item)
{   
}

bool ChattingRecordView::eventFilter(QObject *object, QEvent *event)
{
    if(object == m_scroll && event->type() == QEvent::Enter){
        /*if max > 0, then don't show scroll bar*/
        m_scroll->verticalScrollBar()->setHidden(!m_scroll->verticalScrollBar()->maximum());
    }
    else if(object == m_scroll && event->type() == QEvent::Leave){
        m_scroll->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(object, event);
}

void ChattingRecordView::paintEvent(QPaintEvent *event)
{

    QWidget::paintEvent(event);
}
