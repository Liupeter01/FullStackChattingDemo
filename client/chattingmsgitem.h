#ifndef CHATTINGMSGITEM_H
#define CHATTINGMSGITEM_H
#include <QFont>
#include <QLabel>
#include <QWidget>
#include <QPixmap>
#include <QGridLayout>
#include <QSpacerItem>
#include "msgbubblebase.h"

/*
 * chattingmsgbubble is one of the component in ChattingMsgItem
 */
class ChattingMsgItem
    : public QWidget
{
    Q_OBJECT

public:
    explicit ChattingMsgItem(ChattingRole role, QWidget *parent = nullptr);
    ~ChattingMsgItem();

public:
    void setupUserName(const QString& name);
    void setupIconPixmap(const QPixmap &icon);
    void setupBubbleWidget(QWidget *bubble);

    static constexpr std::size_t icon_width = 45;
    static constexpr std::size_t icon_height = 45;

signals:

private:
    QFont m_font;
    ChattingRole m_role;
    QLabel* m_nameLabel;    /*display name*/
    QLabel* m_iconLabel;    /*display user avator*/
    QGridLayout *m_grid;    /*grid layout*/
    QSpacerItem *m_spacer;
    QWidget* m_bubble;
};

#endif // CHATTINGMSGITEM_H
