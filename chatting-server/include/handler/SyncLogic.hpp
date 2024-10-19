#pragma once
#ifndef _SYNCLOGIC_HPP_
#define _SYNCLOGIC_HPP_
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <network/def.hpp>
#include <queue>
#include <memory>
#include <server/Session.hpp>
#include <singleton/singleton.hpp>
#include <thread>
#include <optional>
#include <unordered_map>

/*declaration*/
struct UserNameCard;

class SyncLogic : public Singleton<SyncLogic> {
  friend class Singleton<SyncLogic>;

public:
  using Convertor = std::function<unsigned short(unsigned short)>;
  using SessionPtr = std::shared_ptr<Session>;
  using NodePtr = std::unique_ptr<RecvNode<std::string, Convertor>>;
  using pair = std::pair<SessionPtr, NodePtr>;

private:
  using CallbackFunc =
      std::function<void(ServiceType, std::shared_ptr<Session>, NodePtr)>;

public:
  ~SyncLogic();
  void commit(pair recv_node);

private:
  SyncLogic();

  /*SyncLogic Class Operations*/
  void shutdown();
  void processing();
  void registerCallbacks();
  void execute(pair &&node);
  void generateErrorMessage(const std::string &log, ServiceType type,
                            ServiceStatus status, SessionPtr conn);

  void incrementConnection();
  void decrementConnection();

  /*Execute Operations*/
  void handlingLogin(ServiceType srv_type, std::shared_ptr<Session> session,
                     NodePtr recv);
  void handlingLogout(ServiceType srv_type, std::shared_ptr<Session> session,
                      NodePtr recv);

  /*
  * get user's basic info(name, age, sex, ...) from redis
  * 1. we are going to search for info inside redis first, if nothing found, then goto 2
  * 2. searching for user info inside mysql
  */
  std::optional<std::unique_ptr<UserNameCard>> getUserBasicInfo(const std::string& key);

public:
          /*redis*/
          static std::string redis_server_login;

          /*store user base info in redis*/
          static std::string user_prefix;

private:
  std::atomic<bool> m_stop;

  /*working thread, handling commited request*/
  std::thread m_working;

  /*mutex & cv => thread safety*/
  std::mutex m_mtx;
  std::condition_variable m_cv;

  /*user commit data to the queue*/
  std::queue<pair> m_queue;

  /*callback list*/
  std::unordered_map<ServiceType, CallbackFunc> m_callbacks;
};

#endif //_SYNCLOGIC_HPP_
