#include <config/ServerConfig.hpp>
#include <spdlog/spdlog.h>
#include <sql/MySQLConnectionPool.hpp>

mysql::MySQLConnectionPool::MySQLConnectionPool() noexcept
    : MySQLConnectionPool(ServerConfig::get_instance()->MySQL_timeout,
                          ServerConfig::get_instance()->MySQL_username,
                          ServerConfig::get_instance()->MySQL_passwd,
                          ServerConfig::get_instance()->MySQL_database,
                          ServerConfig::get_instance()->MySQL_host,
                          ServerConfig::get_instance()->MySQL_port) {
  spdlog::info("Connecting to MySQL service ip: {0}, port: {1}, database: {2}",
               ServerConfig::get_instance()->MySQL_username,
               ServerConfig::get_instance()->MySQL_passwd,
               ServerConfig::get_instance()->MySQL_database);
}

mysql::MySQLConnectionPool::MySQLConnectionPool(
    std::size_t timeOut, const std::string &username,
    const std::string &password, const std::string &database,
    const std::string &host, const std::string &port) noexcept
    : m_timeout(timeOut), m_username(username), m_password(password),
      m_database(database), m_host(host), m_port(port) {
  registerSQLStatement();

  for (std::size_t i = 0; i < m_queue_size; ++i) {
    m_stub_queue.push(std::move(std::make_unique<mysql::MySQLConnection>(
        username, password, database, host, port, this)));
  }

  // m_RRThread = std::thread([this]() {
  //           while (!m_stop)
  //           {
  //                     roundRobinChecking(m_timeout);

  //                    /*suspend this thread by timeout setting*/
  //                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  //          }
  // });
}

mysql::MySQLConnectionPool::~MySQLConnectionPool() {
  /*terminate thread*/
  // if (m_RRThread.joinable()) {
  //           m_RRThread.join();
  // }
}

void mysql::MySQLConnectionPool::registerSQLStatement() {
  m_sql.insert(std::pair(MySQLSelection::HEART_BEAT, fmt::format("SELECT 1")));
  m_sql.insert(
      std::pair(MySQLSelection::FIND_EXISTING_USER,
                fmt::format("SELECT * FROM user_info WHERE {} = ? AND {} = ?",
                            std::string("username"), std::string("email"))));

  m_sql.insert(std::pair(
      MySQLSelection::CREATE_NEW_USER,
      fmt::format("INSERT INTO user_info ({},{},{},{}) VALUES (? ,? ,? ,?)",
                  std::string("username"), std::string("password"),
                  std::string("uid"), std::string("email"))));

  m_sql.insert(std::pair(MySQLSelection::ACQUIRE_NEW_UID,
                         fmt::format("SELECT uid FROM chatting.uid_gen")));
  m_sql.insert(std::pair(MySQLSelection::UPDATE_UID_COUNTER,
                         fmt::format("UPDATE uid_gen SET uid = uid + 1")));
  m_sql.insert(std::pair(
      MySQLSelection::UPDATE_USER_PASSWD,
      fmt::format("UPDATE user_info SET {} = ? WHERE {} = ? AND {} = ?",
                  std::string("password"), std::string("username"),
                  std::string("email"))));
  m_sql.insert(
      std::pair(MySQLSelection::USER_LOGIN_CHECK,
                fmt::format("SELECT uid FROM user_info WHERE {} = ? AND {} = ?",
                            std::string("username"), std::string("password"))));
}

void mysql::MySQLConnectionPool::roundRobinChecking(std::size_t timeout) {
  std::unique_lock<std::mutex> _lckg(m_mtx);
  m_cv.wait(_lckg, [this]() { return !m_stub_queue.empty() || m_stop; });

  if (m_stop) {
    return;
  }

  for (std::size_t i = 0; i < m_stub_queue.size(); ++i) {
    connection::ConnectionRAII<mysql::MySQLConnectionPool,
                               mysql::MySQLConnection>
        instance;

    [[maybe_unused]] bool status = instance->get()->checkTimeout(
        std::chrono::steady_clock::now(), m_timeout);

    /*checktimeout error, then create a new connection*/
    if (!status) [[unlikely]] {
      /*create a new connection*/
      m_stub_queue.push(std::move(std::make_unique<mysql::MySQLConnection>(
          m_username, m_password, m_database, m_host, m_port, this)));

      m_cv.notify_one();
    }
  }
}
