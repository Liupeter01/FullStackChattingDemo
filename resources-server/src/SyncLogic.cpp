#include <config/ServerConfig.hpp>
#include <handler/SyncLogic.hpp>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <server/AsyncServer.hpp>
#include <spdlog/spdlog.h>
#include <tools/tools.hpp>

/*redis*/
std::string SyncLogic::redis_server_login = "redis_server";

/*store user base info in redis*/
std::string SyncLogic::user_prefix = "user_info_";

/*store the server name that this user belongs to*/
std::string SyncLogic::server_prefix = "uuid_";

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
                    std::bind(&SyncLogic::handlingFileUploading, this, std::placeholders::_1,
                              std::placeholders::_2, std::placeholders::_3)));
}

void SyncLogic::commit(pair recv_node) {
  std::lock_guard<std::mutex> _lckg(m_mtx);
  if (m_queue.size() > ServerConfig::get_instance()->ResourceQueueSize) {
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

void SyncLogic::shutdown() {
  m_stop = true;
  m_cv.notify_all();

  /*join the working thread*/
  if (m_working.joinable()) {
    m_working.join();
  }
}

void SyncLogic::handlingFileUploading(ServiceType srv_type,
                                                                  std::shared_ptr<Session> session, 
                                                                  NodePtr recv) {

}