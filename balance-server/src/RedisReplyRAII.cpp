#include <redis/RedisReplyRAII.hpp>

bool redis::RedisReply::checkError() {
  /*return value is a nullptr or is a REDIS_REPLY_ERROR!*/
  if (m_redisReply.get() == nullptr) {
    return false;
  }

  /*error occured*/
  if (m_redisReply->type == REDIS_REPLY_ERROR) {
    return false;
  }

  return true;
}

std::optional<long long> redis::RedisReply::getInterger() const {
  if (m_redisReply.get() != nullptr) {
    return m_redisReply->integer;
  }
  return std::nullopt;
}

std::optional<int> redis::RedisReply::getType() const {
  if (m_redisReply.get() != nullptr) {
    return m_redisReply->type;
  }
  return std::nullopt;
}

std::optional<std::string> redis::RedisReply::getMessage() const {
  if (m_redisReply.get() != nullptr) {
    return m_redisReply->str;
  }
  return std::nullopt;
}
