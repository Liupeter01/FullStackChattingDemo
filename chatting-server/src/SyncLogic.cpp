#include <config/ServerConfig.hpp>
#include <grpc/GrpcBalanceService.hpp>
#include <handler/SyncLogic.hpp>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <spdlog/spdlog.h>
#include <sql/MySQLConnectionPool.hpp>

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
  ServiceType type = static_cast<ServiceType>(node.second->get_id().value());
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
                              std::shared_ptr<Session> session,
                              std::unique_ptr<RecvNode<std::string>> recv) {
  Json::Value src_root;  /*store json from client*/
  Json::Value send_root; /*write into body*/
  Json::Reader reader;

  std::optional<std::string> body = recv->get_msg_body();
  /*recv message error*/
  if (!body.has_value()) {

    return;
  }

  /*parse error*/
  if (!reader.parse(body.value(), src_root)) {

    return;
  }

  std::size_t uuid = src_root["uuid"].asUInt();
  std::string token = src_root["token"].asString();
  spdlog::info("[UUID = {}] Trying to login to ChattingServer with Token {}",
               uuid, token);

  auto response = gRPCBalancerService::userLoginToServer(uuid, token);
  send_root["error"] = response.error();
  if (response.error() !=
      static_cast<std::size_t>(ServiceStatus::SERVICE_SUCCESS)) {
    return;
  }

  /*MYSQL(check uuid)*/
  connection::ConnectionRAII<mysql::MySQLConnectionPool, mysql::MySQLConnection>
      mysql;
}

void SyncLogic::handlingLogout(ServiceType srv_type,
                               std::shared_ptr<Session> session,
                               std::unique_ptr<RecvNode<std::string>> recv) {}

void SyncLogic::shutdown() {
  m_stop = true;
  m_cv.notify_all();

  /*join the working thread*/
  if (m_working.joinable()) {
    m_working.join();
  }
}
