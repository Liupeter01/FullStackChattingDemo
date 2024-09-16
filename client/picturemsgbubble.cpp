#include "picturemsgbubble.h"


PictureMsgBubble::PictureMsgBubble(ChattingRole role,const QPixmap &picture, QWidget *parent)
    :m_picture(new QLabel)
    , m_picSize(PICTURE_WIDTH, PICTURE_HEIGHT)
    , MsgBubbleBase(role, parent)
{
    /*setup bubble widget for base class*/
    this->setupBubbleWidget(m_picture);

    /*fix picture size*/
    this->setupPixmap(picture);
}

PictureMsgBubble::~PictureMsgBubble()
{
}

void PictureMsgBubble::setupPixmap(const QPixmap &pic)
{
    QPixmap fixed = pic.scaled(m_picSize, Qt::KeepAspectRatio);
    m_picture->setScaledContents(true);
    m_picture->setPixmap(fixed);

    auto left_margin = this->layout()->contentsMargins().left();
    auto right_margin = this->layout()->contentsMargins().right();
    auto up_down_margin = this->layout()->contentsMargins().bottom();

    m_picture->setMaximumSize(fixed.width() + left_margin + right_margin, fixed.height() + 2 * up_down_margin);
}
