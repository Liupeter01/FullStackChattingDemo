#ifndef CHATTINGCONTACTLIST_H
#define CHATTINGCONTACTLIST_H

#include <QEvent>
#include <optional>
#include <QListWidget>
#include "mainframeshowlists.h"

/*declaration*/
class QListWidgetItem;
class ChattingContactItem;
struct UserNameCard;

class ChattingContactList : public MainFrameShowLists {
  Q_OBJECT

public:
  ChattingContactList(QWidget *parent = nullptr);
  virtual ~ChattingContactList();

public:
  /*set AddUserWidget*/
  void addAddUserWidget();

  /*set chatting contact info*/
  void addChattingContact(std::shared_ptr<UserNameCard> info);

  /*set dialog with seperator*/
  void addGroupSeperator(const QString &text);

signals:
  /*connect with ChattingDlgMainFrame::switchNewUserPage*/
  void signal_switch_addnewuser();

private slots:
  /*user click one of the contact*/
  void slot_itemClicked(QListWidgetItem *item);

  /*server be able to send authenticate friend list to this client*/
  void slot_init_auth_friend_list();

  /* processing authenticate friend namecard info */
  void slot_signal_add_authenticate_friend(std::optional<std::shared_ptr<UserNameCard>> info);

private:
  /*load contact test func*/
  void loadContactsTest();

  /*signal<=>slot*/
  void registerSignal();

private:
  const QString static_text;
};

#endif // CHATTINGCONTACTLIST_H
