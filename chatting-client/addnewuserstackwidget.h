#ifndef ADDNEWUSERSTACKWIDGET_H
#define ADDNEWUSERSTACKWIDGET_H

#include <QWidget>
#include <unordered_map>

namespace Ui {
class AddNewUserStackWidget;
}

/*declare*/
class AddUserNameCardWidget;
struct UserNameCard;
struct UserFriendRequest;

class AddNewUserStackWidget : public QWidget {
  Q_OBJECT

public:
  explicit AddNewUserStackWidget(QWidget *parent = nullptr);
  virtual ~AddNewUserStackWidget();

public:
  /*add new user widget to list*/
  void addNewWidgetItem(std::shared_ptr<UserFriendRequest> info);

private:
  void registerSignal();

signals:

private slots:
    void slot_incoming_friend_request(std::optional<std::shared_ptr<UserFriendRequest>> info);

private:
  Ui::AddNewUserStackWidget *ui;
  std::unordered_map<
      /*uuid*/
      std::size_t,

      /*namecard widget*/
      std::shared_ptr<AddUserNameCardWidget>>
      m_friendList;
};

#endif // ADDNEWUSERSTACKWIDGET_H
