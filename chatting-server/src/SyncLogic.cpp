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
  Json::Value send_root; /*write into body*/
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
  send_root["error"] = response.error();

  if (response.error() !=
      static_cast<std::size_t>(ServiceStatus::SERVICE_SUCCESS)) {
    spdlog::error("[UUID = {}] Trying to login to ChattingServer Failed!",
                  uuid);
    generateErrorMessage("Internel Server Error",
                         ServiceType::SERVICE_LOGINRESPONSE,
                         ServiceStatus::LOGIN_UNSUCCESSFUL, session);
    return;
  }

  /* 1. check account info inside memory, if not exists then goto 2
   * 2. check account info inside MYSQL(check uuid),
   * if there is no data inside then return error message*/

  auto it = session->s_gate->m_authusers.find(session->s_uuid);
  if (it == session->s_gate->m_authusers.end()) { /*find nothing in memory*/
    connection::ConnectionRAII<mysql::MySQLConnectionPool,
                               mysql::MySQLConnection>
        mysql;

    /*user info not found!*/
    if (!mysql->get()->checkUUID(uuid)) {
      spdlog::error("[UUID = {}] No User Account Found!", uuid);
      generateErrorMessage("No User Account Found",
                           ServiceType::SERVICE_LOGINRESPONSE,
                           ServiceStatus::LOGIN_INFO_ERROR, session);
      return;
    }

    // now insert the new user into the auth users
    try {

      /*we have to lock it because we are going to insert new node*/
      {
        std::lock_guard<std::mutex> _lckg(session->s_gate->m_mtx);

        session->s_gate->m_authusers.insert(
            std::make_pair(session->s_uuid, uuid));
      }
      send_root["uuid"] = std::to_string(uuid);

    } catch (const std::exception &e) {
      spdlog::error("[UUID = {}]Insert new user error {}", uuid, e.what());
      send_root["error"] =
          static_cast<uint8_t>(ServiceStatus::LOGIN_UNSUCCESSFUL);
    }
    session->sendMessage(ServiceType::SERVICE_LOGINRESPONSE,
                         send_root.toStyledString());
  }
}

void SyncLogic::handlingLogout(ServiceType srv_type,
                               std::shared_ptr<Session> session, NodePtr recv) {
}

void SyncLogic::shutdown() {
  m_stop = true;
  m_cv.notify_all();

  /*join the working thread*/
  if (m_working.joinable()) {
    m_working.join();
  }
}
