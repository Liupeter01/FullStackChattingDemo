#include <QDebug>
#include <chrono>
#include "countdownbutton.h"

CountDownButton::CountDownButton(QWidget *parent, std::size_t seconds)
    : m_counter(seconds)
    , m_countdown(seconds)
    , m_timer(new QTimer(this))
    , QPushButton(parent)
{
    registerTimer(seconds);
}

CountDownButton::~CountDownButton()
{
    m_timer->stop();
}

void CountDownButton::registerTimer(std::size_t seconds)
{
    connect(m_timer, &QTimer::timeout, [this](){
        --m_counter;

        /*reset data display*/
        if(m_counter <= 0){
            m_timer->stop();
            m_counter = m_countdown;

            this->setText(QString("Get Verification Code"));
            this->setEnabled(true);
            return;
        }

        this->setText(QString::number(m_counter));
    });
}

void CountDownButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton){
        qDebug() << "LeftButton Release, count down start!";

        this->setText(QString::number(m_countdown));
        this->setEnabled(false);

        //auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::seconds(m_countdown));

        m_timer->start(m_countdown * 100);  //seconds->microsecond
        emit clicked();
    }

    QPushButton::mouseReleaseEvent(e);
}
