#include <thread>
#include <spdlog/spdlog.h>
#include <config/ServerConfig.hpp>
#include <redis/RedisManager.hpp>

redis::details::RedisManager::RedisManager::RedisManager()
    : RedisManager(std::thread::hardware_concurrency() < 2
                       ? 2
                       : std::thread::hardware_concurrency(),
                   ServerConfig::get_instance()->Redis_ip_addr,
                   ServerConfig::get_instance()->Redis_port,
                   ServerConfig::get_instance()->Redis_passwd) 
{
          spdlog::info("Connecting to Redis service ip: {0}, port: {1}", 
                    ServerConfig::get_instance()->Redis_ip_addr.c_str(),
                    ServerConfig::get_instance()->Redis_port
          );
}

redis::details::RedisManager::RedisManager::RedisManager(
    std::size_t poolSize, const std::string &ip, unsigned short port,
    const std::string &password)
    : m_stop(false) {
  for (std::size_t i = 0; i < poolSize; ++i) {
    this->m_contextPool.push(
        std::move(std::make_unique<context>(ip, port, password)));
  }
}

redis::details::RedisManager::~RedisManager() { shutdown(); }

void redis::details::RedisManager::shutdown() {
  /*terminate pool*/
  m_stop = true;
  m_cv.notify_all();

  std::lock_guard<std::mutex> _lckg(m_mtx);
  while (!m_contextPool.empty()) {
    m_contextPool.pop();
  }
}

std::optional<redis::details::RedisManager::context_ptr>
redis::details::RedisManager::acquire() {
  std::unique_lock<std::mutex> _lckg(m_mtx);
  m_cv.wait(_lckg, [this]() { return !m_contextPool.empty() || m_stop; });

  /*check m_stop flag*/
  if (m_stop) {
    return std::nullopt;
  }
  context_ptr temp = std::move(m_contextPool.front());
  m_contextPool.pop();
  return temp;
}

void redis::details::RedisManager::release(
    redis::details::RedisManager::context_ptr stub) {
  if (m_stop) {
    return;
  }
  std::lock_guard<std::mutex> _lckg(m_mtx);
  m_contextPool.push(std::move(stub));
  m_cv.notify_one();
}

redis::RedisRAII::RedisRAII() : status(true) {
  auto optional = redis::details::RedisManager::get_instance()->acquire();
  if (!optional.has_value()) {
    status = false;
  } else {
    m_redis = std::move(optional.value());
  }
}

redis::RedisRAII::~RedisRAII() {
  if (status) {
    redis::details::RedisManager::get_instance()->release(std::move(m_redis));
    status = false;
  }
}

std::optional<redis::RedisRAII::wrapper> redis::RedisRAII::operator->() {
  if (status) {
    return wrapper(m_redis.get());
  }
  return std::nullopt;
}
