#pragma once
#ifndef _REDISMANAGER_HPP_
#define _REDISMANAGER_HPP_
#include <condition_variable>
#include <mutex>
#include <queue>
#include <redis/RedisReplyRAII.hpp>
#include <singleton/singleton.hpp>

namespace redis {
namespace details {
class RedisManager : public Singleton<RedisManager> {
  friend class Singleton<RedisManager>;

public:
  using context = redis::RedisContext;
  using context_ptr = std::unique_ptr<context>;

  ~RedisManager();
  void shutdown();
  std::optional<context_ptr> acquire();
  void release(context_ptr stub);

private:
  RedisManager();
  RedisManager(std::size_t poolSize, const std::string &ip, unsigned short port,
               const std::string &password);

private:
  /*stop flag*/
  std::atomic<bool> m_stop;
  std::mutex m_mtx;
  std::condition_variable m_cv;
  std::queue<context_ptr> m_contextPool;
};
} // namespace details

class RedisRAII {
  using wrapper = tools::ResourcesWrapper<details::RedisManager::context>;

public:
  RedisRAII();
  ~RedisRAII();
  std::optional<wrapper> operator->();

private:
  bool status; // load stub success flag
  details::RedisManager::context_ptr m_redis;
};
} // namespace redis

#endif
