#ifndef PICTUREMSGBUBBLE_H
#define PICTUREMSGBUBBLE_H

#include <QLabel>
#include <QPixmap>
#include "msgbubblebase.h"

class PictureMsgBubble : public MsgBubbleBase
{
    Q_OBJECT
public:
    PictureMsgBubble(ChattingRole role, const QPixmap &picture, QWidget* parent = nullptr);
    ~PictureMsgBubble();

public:
    static constexpr std::size_t PICTURE_WIDTH = 160;
    static constexpr std::size_t PICTURE_HEIGHT = 90;

private:
    void setupPixmap(const QPixmap &pic);

private:
    QSize m_picSize;
    QLabel* m_picture;
};

#endif // PICTUREMSGBUBBLE_H
