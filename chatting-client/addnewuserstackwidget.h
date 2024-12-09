#ifndef ADDNEWUSERSTACKWIDGET_H
#define ADDNEWUSERSTACKWIDGET_H

#include <QWidget>
#include <unordered_map>

namespace Ui {
class AddNewUserStackWidget;
}

/*declare*/
struct UserNameCard;
struct UserFriendRequest;
class AddUserNameCardWidget;
class LoadingWaitDialog;
class ChattingDlgMainFrame;
class NameCardWidgetShowList;

class AddNewUserStackWidget : public QWidget {
  Q_OBJECT
    friend class ChattingDlgMainFrame;

public:
  explicit AddNewUserStackWidget(QWidget *parent = nullptr);
  virtual ~AddNewUserStackWidget();

public:
  /*load limited amount of request list*/
  void loadLimitedReqList();

  /*add new user widget to list*/
  void addNewWidgetItem(std::shared_ptr<UserFriendRequest> info);

private:
  void registerSignal();

  /*expose ui interface for chattingdlgmainframe*/
  NameCardWidgetShowList* getFriendListUI() const;

private slots:
  /*server send friend request list to this client when user just finish login*/
    void slot_init_friend_request_list();

    void slot_incoming_friend_request(std::optional<std::shared_ptr<UserFriendRequest>> info);

private:
  Ui::AddNewUserStackWidget *ui;

  /* define how many chat recoreds are going to show up on chat record list*/
  static std::size_t FRIENDREQ_PER_PAGE;

  /* friend request */
  std::size_t m_curr_friend_requests_loaded = 0;
};

#endif // ADDNEWUSERSTACKWIDGET_H
