#ifndef CHATTINGCONTACTLIST_H
#define CHATTINGCONTACTLIST_H

#include "mainframeshowlists.h"
#include <QEvent>
#include <QListWidget>

/*declaration*/
class QListWidgetItem;
class ChattingContactItem;

class ChattingContactList : public MainFrameShowLists {
  Q_OBJECT

public:
  ChattingContactList(QWidget *parent = nullptr);
  virtual ~ChattingContactList();

public:
  /*set AddUserWidget*/
  void addAddUserWidget();

  /*set chatting contact info*/
  void addChattingContact(const QString &target_picture, const QString &text);

  /*set dialog with seperator*/
  void addGroupSeperator(const QString &text);

signals:
  /*connect with ChattingDlgMainFrame::switchNewUserPage*/
  void signal_switch_addnewuser();

private slots:
  /*user click one of the contact*/
  void slot_itemClicked(QListWidgetItem *item);

private:
  /*load contact test func*/
  void loadContactsTest();

  /*signal<=>slot*/
  void registerSignal();

private:
  const QString static_text;
};

#endif // CHATTINGCONTACTLIST_H
