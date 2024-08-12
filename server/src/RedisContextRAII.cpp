#include<redis/RedisContextRAII.hpp>
#include<redis/RedisReplyRAII.hpp>

redis::RedisContext::RedisContext() noexcept
		  :m_valid(false)
		  ,m_redisContext(nullptr)
{
}

redis::RedisContext::RedisContext(const std::string& ip,
														unsigned short port,
														const std::string& password) noexcept
		  :m_valid(false)
		  , m_redisContext(redisConnect(ip.c_str(), port))
{
		  /*error occured*/
		  if (!checkError()) {
					m_redisContext.reset();
		  }
		  else {
					printf("Connection to Redis server success! \n");
					checkAuth(password);
		  }
}

bool redis::RedisContext::isValid()
{
		  return m_valid;
}

bool redis::RedisContext::setValue(const std::string& key, const std::string& value)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  auto status = m_replyDelegate->redisCommand(*this, std::string("SET %s %s"), key.c_str(), value.c_str());
		  if (status) {
					if (m_replyDelegate->getMessage().has_value() &&
							  (m_replyDelegate->getMessage().value() == "ok"
										|| m_replyDelegate->getMessage().value() == "OK")) 
					{
							  printf("[REDIS]: excute command [ SET key = %s, value = %s] successfully!\n", key.c_str(), value.c_str());
							  return true;
					}
		  }
		  return false;
}

bool redis::RedisContext::leftPush(const std::string& key, const std::string& value)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  auto status = m_replyDelegate->redisCommand(*this, std::string("LPUSH %s %s"), key.c_str(), value.c_str());
		  if (status) {
					if (m_replyDelegate->getType().has_value() 
							  && m_replyDelegate->getType().value() == REDIS_REPLY_INTEGER
							  && m_replyDelegate->getInterger().has_value()
							  && m_replyDelegate->getInterger().value() > 0)
					{
							  printf("[REDIS]: excute command [ LPUSH key = %s, value = %s] successfully!\n", key.c_str(), value.c_str());
							  return true;
					}
		  }
		  return false;
}

bool redis::RedisContext::rightPush(const std::string& key, const std::string& value)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  auto status = m_replyDelegate->redisCommand(*this, std::string("RPUSH %s %s"), key.c_str(), value.c_str());
		  if (status) {
					if (m_replyDelegate->getType().has_value()
							  && m_replyDelegate->getType().value() == REDIS_REPLY_INTEGER
							  && m_replyDelegate->getInterger().has_value()
							  && m_replyDelegate->getInterger().value() > 0)
					{
							  printf("[REDIS]: excute command [ RPUSH key = %s, value = %s] successfully!\n", key.c_str(), value.c_str());
							  return true;
					}
		  }
		  return false;
}

bool redis::RedisContext::delPair(const std::string& key)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  auto status = m_replyDelegate->redisCommand(*this, std::string("DEL %s"), key.c_str());
		  if (status) {
					if (m_replyDelegate->getType().has_value()
							  && m_replyDelegate->getType().value() == REDIS_REPLY_INTEGER)
					{
							  printf("[REDIS]: excute command [ DEL key = %s ] successfully!\n", key.c_str());
							  return true;
					}
		  }
		  return false;
}

bool  redis::RedisContext::existKey(const std::string& key)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  auto status = m_replyDelegate->redisCommand(*this, std::string("exists %s"), key.c_str());
		  if (status) {
					if (m_replyDelegate->getType().has_value()
							  && m_replyDelegate->getType().value() == REDIS_REPLY_INTEGER
							  && m_replyDelegate->getInterger().has_value()
							  && !m_replyDelegate->getInterger().value())
					{
							  printf("[REDIS]: excute command [ exists key = %s ] successfully!\n", key.c_str());
							  return true;
					}
		  }
		  return false;
}

std::optional<std::string> redis::RedisContext::checkValue(const std::string &key)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  if (!m_replyDelegate->redisCommand(*this, std::string("GET %s"), key.c_str())) {
					return std::nullopt;
		  }
		  if (m_replyDelegate->getType().has_value() && m_replyDelegate->getType().value() != REDIS_REPLY_STRING) {
					return std::nullopt;
		  }
		  printf("[REDIS]: excute command [ GET key = %s ] successfully!\n", key.c_str());
		  return m_replyDelegate->getMessage();
}

std::optional<std::string> redis::RedisContext::leftPop(const std::string& key)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  if (!m_replyDelegate->redisCommand(*this, std::string("LPOP %s"), key.c_str())) {
					return std::nullopt;
		  }
		  if (!m_replyDelegate->getMessage().has_value()) {
					return std::nullopt;
		  }
		  if (m_replyDelegate->getType().has_value()
					&& m_replyDelegate->getType().value() == REDIS_REPLY_NIL)
		  {
					return std::nullopt;
		  }
		  printf("[REDIS]: excute command [ LPOP key = %s ] successfully!\n", key.c_str());
		  return m_replyDelegate->getMessage();
}

std::optional<std::string> redis::RedisContext::rightPop(const std::string& key)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  if (!m_replyDelegate->redisCommand(*this, std::string("RPOP %s"), key.c_str())) {
					return std::nullopt;
		  }
		  if (m_replyDelegate->getType().has_value()
					&& m_replyDelegate->getType().value() == REDIS_REPLY_NIL) 
		  {
					return std::nullopt;
		  }
		  printf("[REDIS]: excute command [ RPOP key = %s ] successfully!\n", key.c_str());
		  return m_replyDelegate->getMessage();
}

bool redis::RedisContext::checkError()
{
		  if (m_redisContext.get() == nullptr) {
					printf("Connection to Redis server failed! No instance!\n");
					return m_valid;		//false;
		  }

		  /*error occured*/
		  if (m_redisContext->err) {
					printf("Connection to Redis server failed! error code: %s\n", m_redisContext->errstr);
					return m_valid;
		  }

		  m_valid = true;
		  return m_valid;
}

bool redis::RedisContext::checkAuth(std::string_view sv)
{
		  std::unique_ptr<RedisReply> m_replyDelegate = std::make_unique<RedisReply>();
		  auto status = m_replyDelegate->redisCommand(*this, std::string("AUTH %s"), sv.data());
		  if (status) {
					printf("[REDIS]: excute command [ AUTH ] successfully!\n");
		  }
		  return status;
}

std::optional<tools::RedisContextWrapper> redis::RedisContext::operator->()
{
		  if (isValid()) {
					return tools::RedisContextWrapper(m_redisContext.get());
		  }
		  return std::nullopt;
}