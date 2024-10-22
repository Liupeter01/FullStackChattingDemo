#ifndef USERACCOUNTMANAGER_H
#define USERACCOUNTMANAGER_H

#include "singleton.hpp"
#include <QString>

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

private:
  UserAccountManager() : m_info() {}

private:
  struct ChattingServerInfo {
    QString uuid;
    QString host;
    QString port;
    QString token;
  } m_info;
};

#endif // USERACCOUNTMANAGER_H
