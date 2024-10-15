#include <config/ServerConfig.hpp>
#include <grpc/GrpcBalanceService.hpp>
#include <handler/SyncLogic.hpp>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <server/AsyncServer.hpp>
#include <spdlog/spdlog.h>
#include <sql/MySQLConnectionPool.hpp>
#include <tools/tools.hpp>
#include <server/UserManager.hpp>
#include <server/UserNameCard.hpp>
#include <redis/RedisManager.hpp>

/*redis*/
std::string SyncLogic::redis_server_login = "redis_server";

/*store user base info in redis*/
std::string SyncLogic::user_prefix = "user_info_";

SyncLogic::SyncLogic() : m_stop(false) {
  /*register callbacks*/
  registerCallbacks();

  /*start processing thread to process queue*/
  m_working = std::thread(&SyncLogic::processing, this);
}

SyncLogic::~SyncLogic() { shutdown(); }

void SyncLogic::registerCallbacks() {
  m_callbacks.insert(std::pair<ServiceType, CallbackFunc>(
      ServiceType::SERVICE_LOGINSERVER,
      std::bind(&SyncLogic::handlingLogin, this, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3)));
}

void SyncLogic::commit(pair recv_node) {
  std::lock_guard<std::mutex> _lckg(m_mtx);
  if (m_queue.size() > ServerConfig::get_instance()->ChattingServerQueueSize) {
    spdlog::warn("SyncLogic Queue is full!");
    return;
  }
  m_queue.push(std::move(recv_node));
  m_cv.notify_one();
}

void SyncLogic::generateErrorMessage(const std::string &log, ServiceType type,
                                     ServiceStatus status, SessionPtr conn) {

  Json::Value root;
  spdlog::error(log);
  root["error"] = static_cast<uint8_t>(status);
  conn->sendMessage(type, root.toStyledString());
}

void SyncLogic::processing() {
  for (;;) {
    std::unique_lock<std::mutex> _lckg(m_mtx);
    m_cv.wait(_lckg, [this]() { return m_stop || !m_queue.empty(); });

    if (m_stop) {
      /*take care of the rest of the tasks, and shutdown synclogic*/
      while (!m_queue.empty()) {
        /*execute callback functions*/
        execute(std::move(m_queue.front()));
        m_queue.pop();
      }
      return;
    }

    auto &front = m_queue.front();
    execute(std::move(m_queue.front()));
    m_queue.pop();
  }
}

void SyncLogic::execute(pair &&node) {
  std::shared_ptr<Session> session = node.first;
  ServiceType type = static_cast<ServiceType>(node.second->_id);
  try {
    /*executing callback on specific type*/
    auto it = m_callbacks.find(type);
    if (it == m_callbacks.end()) {
      spdlog::error("Service Type Not Found!");
      return;
    }
    m_callbacks[type](type, session, std::move(node.second));
  } catch (const std::exception &e) {
    spdlog::error("Excute Method Failed, Internel Server Error! Error Code {}",
                  e.what());
  }
}

void SyncLogic::handlingLogin(ServiceType srv_type,
                              std::shared_ptr<Session> session, NodePtr recv) {
  Json::Value src_root;  /*store json from client*/
  Json::Value redis_root; /*write into body*/
  Json::Reader reader;

  std::optional<std::string> body = recv->get_msg_body();
  /*recv message error*/
  if (!body.has_value()) {
    generateErrorMessage("Failed to parse json data",
                         ServiceType::SERVICE_LOGINRESPONSE,
                         ServiceStatus::JSONPARSE_ERROR, session);
    return;
  }

  /*parse error*/
  if (!reader.parse(body.value(), src_root)) {
    generateErrorMessage("Failed to parse json data",
                         ServiceType::SERVICE_LOGINRESPONSE,
                         ServiceStatus::LOGIN_UNSUCCESSFUL, session);
    return;
  }

  /*parsing failed*/
  if (!(src_root.isMember("uuid") && src_root.isMember("token"))) {
    generateErrorMessage("Failed to parse json data",
                         ServiceType::SERVICE_LOGINRESPONSE,
                         ServiceStatus::LOGIN_UNSUCCESSFUL, session);
    return;
  }

  // std::size_t uuid = src_root["uuid"].asInt();
  std::string uuid_str = src_root["uuid"].asString();
  std::string token = src_root["token"].asString();
  spdlog::info("[UUID = {}] Trying to login to ChattingServer with Token {}",
               uuid_str, token);

  auto uuid_optional = tools::string_to_value<std::size_t>(uuid_str);
  if (!uuid_optional.has_value()) {
    generateErrorMessage("Failed to convert string to number",
                         ServiceType::SERVICE_LOGINRESPONSE,
                         ServiceStatus::LOGIN_UNSUCCESSFUL, session);
    return;
  }

  std::size_t uuid = uuid_optional.value();

  auto response = gRPCBalancerService::userLoginToServer(uuid, token);
  redis_root["error"] = response.error();

  if (response.error() !=
      static_cast<std::size_t>(ServiceStatus::SERVICE_SUCCESS)) {
    spdlog::error("[UUID = {}] Trying to login to ChattingServer Failed!",
                  uuid);
    generateErrorMessage("Internel Server Error",
                         ServiceType::SERVICE_LOGINRESPONSE,
                         ServiceStatus::LOGIN_UNSUCCESSFUL, session);
    return;
  }

  /*
  * get user's basic info(name, age, sex, ...) from redis
  * 1. we are going to search for info inside redis first, if nothing found, then goto 2
  * 2. searching for user info inside mysql
  */
  std::optional<std::shared_ptr<UserNameCard>> info_str = getUserBasicInfo(std::to_string(uuid));
  if (!info_str.has_value()) {
            spdlog::error("[UUID = {}] Can not find a single user in MySQL and Redis", uuid);
            generateErrorMessage("No User Account Found",
                      ServiceType::SERVICE_LOGINRESPONSE,
                      ServiceStatus::LOGIN_INFO_ERROR, session
            );
            return;
  }
  else
  {
            /*bind uuid with a session*/
            session->setUUID(uuid_str);

            /* add user uuid and session as a pair and store it inside usermanager */
            UserManager::get_instance()->alterUserSession(uuid_str, session);

            /*returning info to client*/
            std::shared_ptr<UserNameCard> info = info_str.value();
            redis_root["error"] = static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS);
            redis_root["uuid"] = std::to_string(uuid);
            redis_root["sex"] = static_cast<uint8_t>(info->m_sex);
            redis_root["avator"] = info->m_avatorPath;
            redis_root["nickname"] = info->m_nickname;
            redis_root["description"] = info->m_description;

            /*
            * add user connection counter for current server
            * 1. HGET not exist: Current Chatting server didn't setting up connection counter
            * 2. HGET exist: Increment by 1
            */
            connection::ConnectionRAII<redis::RedisConnectionPool, redis::RedisContext> raii;

            /*try to acquire value from redis*/
            std::optional<std::string> counter = raii->get()->getValueFromHash(redis_server_login,
                      ServerConfig::get_instance()->ChattingServerName);

            std::size_t new_number(0);

            /* redis has this value then read it from redis*/
            if (counter.has_value()) { 
                      new_number = tools::string_to_value<std::size_t>(counter.value()).value();
            }

            /*incerment and set value to hash by using HSET*/
            raii->get()->setValue2Hash(redis_server_login,
                      ServerConfig::get_instance()->ChattingServerName, std::to_string(++new_number));

            session->sendMessage(ServiceType::SERVICE_LOGINRESPONSE,
                      redis_root.toStyledString());
  }
}

void SyncLogic::handlingLogout(ServiceType srv_type,
                               std::shared_ptr<Session> session, NodePtr recv) {}

/*get user's basic info(name, age, sex, ...) from redis*/
std::optional<std::unique_ptr<UserNameCard>> SyncLogic::getUserBasicInfo(const std::string& key)
{
          connection::ConnectionRAII<redis::RedisConnectionPool, redis::RedisContext> raii;

          /*
          * Search For Info Cache in Redis
          * find key = user_prefix  + uuid in redis, GET
          */
          std::optional<std::string> info_str = raii->get()->checkValue(user_prefix + key);

          /*we could find it in Redis directly*/
          if (info_str.has_value()) {
                    /*parse cache data inside Redis*/
                    Json::Reader reader;
                    Json::Value root;
                    reader.parse(info_str.value(), root);

                    return  std::make_unique<UserNameCard>(
                              root["uuid"].asString(),
                              root["avator"].asString(),
                              root["nickname"].asString(),
                              root["description"].asString(),
                              static_cast<Sex>(root["sex"].asInt64())
                    );
          }
          else { 
                    Json::Value redis_root;

                    /*search it in mysql*/
                    connection::ConnectionRAII<mysql::MySQLConnectionPool, mysql::MySQLConnection>mysql;

                    /*user info not found!*/
                    std::size_t uuid = tools::string_to_value<std::size_t>(key).value();
                    if (!mysql->get()->checkUUID(uuid)) {
                              spdlog::error("[UUID = {}] No User Account Found!", uuid);
                              return std::nullopt;
                    }

                    //redis_root["error"] = static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS);
                    //redis_root["uuid"] = std::to_string(uuid);
                    //redis_root["sex"] = static_cast<uint8_t>(info->m_sex);
                    //redis_root["avator"] = info->m_avatorPath;
                    //redis_root["nickname"] = info->m_nickname;
                    //redis_root["description"] = info->m_description;

                    raii->get()->setValue(user_prefix + key, redis_root.toStyledString());
          }
          return std::nullopt;
}

void SyncLogic::shutdown() {
  m_stop = true;
  m_cv.notify_all();

  /*join the working thread*/
  if (m_working.joinable()) {
    m_working.join();
  }
}
