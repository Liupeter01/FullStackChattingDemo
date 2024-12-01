#ifndef CHATTINGDLGMAINFRAME_H
#define CHATTINGDLGMAINFRAME_H

#include <QDialog>
#include <QIcon>
#include <QLabel>
#include <QVector>
#include <atomic>
#include <memory>
#include <unordered_map>

class SideBarWidget;
class QMouseEvent;
class QListWidgetItem;
class LoadingWaitDialog;
class UserNameCard;

namespace Ui {
class ChattingDlgMainFrame;
}

class ChattingDlgMainFrame : public QDialog {
  Q_OBJECT

public:
  explicit ChattingDlgMainFrame(QWidget *parent = nullptr);
  virtual ~ChattingDlgMainFrame();

protected:
  /*chat list test*/
  void addItemToChatListTest();

  /*customlized functions*/
  bool eventFilter(QObject *object, QEvent *event) override;

private:
  void registerSignal();

  /*register action for search edit ui item*/
  void registerSearchEditAction();
  void registerSearchEditSignal();
  void updateSearchUserButton();

  /*windows switcher(accroding to m_dlgMode)*/
  void switchRelevantListWidget();

  /*register handler for chatting*/
  void updateMyChat();

  /*register handler for Contact list*/
  void updateMyContact();

  /*delegate sidebar widget*/
  void addLabel(SideBarWidget *widget);
  void resetAllLabels(SideBarWidget *new_widget);

  /*clear search_edit according to mouse position*/
  void clearSearchByMousePos(QMouseEvent *event);

  /* switch to chatting page by using stackedWidget */
  void switchChattingPage();

  /* switch to new user page by using stackedWidget */
  void switchNewUserPage();

  /*wait for remote server data*/
  void waitForDataFromRemote(bool status);

  /*
   * add chatting contact
   * and register current uuid to DS m_chatHisoryWidList
   */
  void addChattingContact(std::shared_ptr<UserNameCard> info);

  /*is there any existing uuid related to this QListWidgetItem*/
  bool alreadyExistInHistoryWidListList(const QString &uuid) const;

private slots:
  /*
   * waiting for data from remote server
   * status = true: activate
   * status = false: deactivate
   */
  void slot_waiting_for_data(bool status);

  /*search text changed*/
  void slot_search_text_changed();

  void slot_load_more_record();

  /*when side bar button activated, then display relevant info on show_list*/
  void slot_display_chat_list();
  void slot_display_contact_list();

  /*
   * user click the item shown in the ListWidget
   * ListItemType::Default
   * When User Start To Searching User ID: ListItemType::SearchUserId
   */
  void slot_list_item_clicked(QListWidgetItem *clicked_item);

  /*server be able to send authenticate friend list to this client*/
  void slot_init_auth_friend_list();

  /*
   * Create a signal<->slot for processing authenticate friend namecard info
   * 1.recieve authenticate friend list from server, then create multiple chatting history widgets
   * 1.recieve signal authenticate friend, then create a chatting history widget
   * TCPNetworkConnection::signal_add_authenticate_friend
   */
  void slot_signal_add_authenticate_friend(std::optional<std::shared_ptr<UserNameCard>> info);

private:
  Ui::ChattingDlgMainFrame *ui;

  /*reserve for search line edit*/
  QAction *m_searchAction;

  /*reserve for cancel user searching*/
  QAction *m_cancelAction;

  QVector<std::shared_ptr<SideBarWidget>> m_qlabelSet;

  /*
   * we use this to store chatting history widget
   * all the chats made by this user will shown here
   */
  std::unordered_map<QString, QListWidgetItem *> m_chatHistoryWidList;

  /*cur qlabel*/
  SideBarWidget *m_curQLabel;

  /*close status dialog*/
  std::shared_ptr<QDialog> m_Dlg;

  /*wait for remote server data status*/
  std::atomic<bool> m_send_status;

  /*LoadingWaitDialog*/
  std::shared_ptr<LoadingWaitDialog> m_loading;

  enum class ChattingDlgMode {
    ChattingDlgChattingMode,  // show multiple user chatting dialog
    ChattingDlgSearchingMode, // allow user to search
    chattingDlgContactMode    // show contacts
  } m_dlgMode;
};

#endif // CHATTINGDLGMAINFRAME_H
