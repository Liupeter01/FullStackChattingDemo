#pragma once
#ifndef _USERMANAGER_HPP_
#define _USERMANAGER_HPP_
#include <mutex>
#include <optional>
#include <singleton/singleton.hpp>
#include <unordered_map>

/*declaration*/
class Session;

class UserManager : public Singleton<UserManager> {
  friend class Singleton<UserManager>;
  UserManager();

public:
  ~UserManager();
  std::optional<std::shared_ptr<Session>> getSession(const std::size_t uuid);
  void removeUsrSession(const std::size_t uuid);
  void alterUserSession(const std::size_t uuid,
                        std::shared_ptr<Session> session);

private:
  std::mutex m_update_mtx;
  std::unordered_map<
      /*uuid*/ std::size_t,
      /*user belonged session*/ std::shared_ptr<Session>>
      m_uuid2Session;
};
#endif //_USERMANAGER_HPP_
