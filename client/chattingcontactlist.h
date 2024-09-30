#ifndef CHATTINGCONTACTLIST_H
#define CHATTINGCONTACTLIST_H

#include <QListWidget>
#include <QEvent>
#include "mainframeshowlists.h"

/*declaration*/
class QListWidgetItem;
class ChattingContactItem;

class ChattingContactList : public MainFrameShowLists
{
    Q_OBJECT

public:
    ChattingContactList(QWidget* parent = nullptr);
    ~ChattingContactList();

public:
    /*set AddUserWidget*/
    void addAddUserWidget();

    /*set chatting contact info*/
    void addChattingContact(const QString &target_picture,const QString& text);

    /*set dialog with seperator*/
    void addGroupSeperator(const QString& text);

signals:

private slots:
    /*user click one of the contact*/
    void slot_itemClicked(QListWidgetItem *item);

private:
    /*load contact test func*/
    void loadContactsTest();

    /*signal<=>slot*/
    void registerSignal();

    /*add widget item to the listwidget*/
    void addItemWidget(ChattingContactItem* new_inserted);

private:
    const QString static_text;
};

#endif // CHATTINGCONTACTLIST_H
