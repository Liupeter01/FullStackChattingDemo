#ifndef USERACCOUNTMANAGER_H
#define USERACCOUNTMANAGER_H

#include "singleton.hpp"
#include <QString>
#include <vector>
#include <UserFriendRequest.hpp>

class QJsonArray;

class UserAccountManager : public Singleton<UserAccountManager> {
  friend class Singleton<UserAccountManager>;

public:
  ~UserAccountManager() {}
  void set_host(const QString &_host) { m_info.host = _host; }
  void set_port(const QString &_port) { m_info.port = _port; }
  void set_token(const QString &_token) { m_info.token = _token; }
  void set_uuid(const QString &_uuid) { m_info.uuid = _uuid; }

  const QString &get_host() const { return m_info.host; }
  const QString &get_port() const { return m_info.port; }
  const QString &get_token() const { return m_info.token; }
  const QString get_uuid() const { return m_info.uuid; }

public:
  void addItem2FriendRequestList(std::shared_ptr<UserFriendRequest> info);
  void appendFriendRequestList(const QJsonArray& array);
  const std::vector<std::shared_ptr<UserFriendRequest>>& getFriendRequestList();
    bool alreadyExist(const QString &uuid) const;

    void setUserInfo(std::shared_ptr<UserNameCard> info);

private:
  UserAccountManager() : m_info() {}

private:
  struct ChattingServerInfo {
    QString uuid;
    QString host;
    QString port;
    QString token;
  } m_info;

  /*store current user's info*/
  std::shared_ptr<UserNameCard> m_userInfo;

  /*store friending requests*/
  std::vector<std::shared_ptr<UserFriendRequest>> m_friend_request_list;

  /*store exisiting friend list*/
};

#endif // USERACCOUNTMANAGER_H
