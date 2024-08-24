#include<spdlog/spdlog.h>
#include<config/ServerConfig.hpp>
#include<sql/MySQLManagement.hpp>

mysql::details::MySQLManagement::MySQLManagement() noexcept
          :MySQLManagement(
                    std::thread::hardware_concurrency() < 2 ? 2 : std::thread::hardware_concurrency(),
                    ServerConfig::get_instance()->MySQL_timeout,
                    ServerConfig::get_instance()->MySQL_username,
                    ServerConfig::get_instance()->MySQL_passwd,
                    ServerConfig::get_instance()->MySQL_database,
                    ServerConfig::get_instance()->MySQL_host,
                    ServerConfig::get_instance()->MySQL_port)
{
          spdlog::info("Connecting to MySQL service ip: {0}, port: {1}, database: {2}",
                    ServerConfig::get_instance()->MySQL_username,
                    ServerConfig::get_instance()->MySQL_passwd,
                    ServerConfig::get_instance()->MySQL_database
          );
}

mysql::details::MySQLManagement::MySQLManagement(std::size_t poolSize,
          std::size_t timeOut,
          const std::string& username,
          const std::string& password,
          const std::string& database,
          const std::string& host,
          const std::string& port) noexcept

          :m_stop(false)
          , m_timeout(timeOut)
          , m_username(username)
          , m_password(password)
          , m_database(database)
          , m_host(host)
          , m_port(port)
{
          registerSQLStatement();

          for (std::size_t i = 0; i < poolSize; ++i) {
                    m_queue.push(std::move(std::make_unique<mysql::MySQLConnection>(
                              username,
                              password,
                              database,
                              host,
                              port,
                              this
                    )));
          }

          //m_RRThread = std::thread([this]() {
          //          while (!m_stop) 
          //          {
          //                    roundRobinChecking(m_timeout);

          //                    /*suspend this thread by timeout setting*/
          //                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
          //          }
          // });
}

mysql::details::MySQLManagement::~MySQLManagement()
{
          shutdown();
}

void mysql::details::MySQLManagement::shutdown()
{
          /*terminate pool*/
          m_stop = true;
          m_ctx.notify_all();

          /*terminate thread*/
          //if (m_RRThread.joinable()) {
          //          m_RRThread.join();
          //}

          std::lock_guard<std::mutex> _lckg(m_mtx);
          while (!m_queue.empty()) {
                    m_queue.pop();
          }
}

std::optional<mysql::details::MySQLManagement::context_ptr> mysql::details::MySQLManagement::acquire()
{
          std::unique_lock<std::mutex> _lckg(m_mtx);
          m_ctx.wait(_lckg, [this]() { return !m_queue.empty() || m_stop; });

          /*check m_stop flag*/
          if (m_stop) {
                    return std::nullopt;
          }
          context_ptr temp = std::move(m_queue.front());
          m_queue.pop();
          return temp;
}

void mysql::details::MySQLManagement::release(mysql::details::MySQLManagement::context_ptr conn)
{
          if (m_stop) {
                    return;
          }
          std::lock_guard<std::mutex> _lckg(m_mtx);
          m_queue.push(std::move(conn));
          m_ctx.notify_one();
}

void mysql::details::MySQLManagement::registerSQLStatement()
{
          m_sql.insert(std::pair(MySQLSelection::HEART_BEAT, fmt::format("SELECT 1")));
          m_sql.insert(std::pair(MySQLSelection::FIND_EXISTING_USER, fmt::format("SELECT * FROM user_info WHERE {} = ? AND {} = ?",
                    std::string("username"),
                    std::string("email")
          )));

          m_sql.insert(std::pair(MySQLSelection::CREATE_NEW_USER, fmt::format("INSERT INTO user_info ({},{},{},{}) VALUES (? ,? ,? ,?)",
                    std::string("username"),
                    std::string("password"),
                    std::string("uid"),
                    std::string("email")
          )));

          m_sql.insert(std::pair(MySQLSelection::ACQUIRE_NEW_UID, fmt::format("SELECT uid FROM chatting.uid_gen")));
          m_sql.insert(std::pair(MySQLSelection::UPDATE_UID_COUNTER, fmt::format("UPDATE uid_gen SET uid = uid + 1")));
          m_sql.insert(std::pair(MySQLSelection::UPDATE_USER_PASSWD, fmt::format("UPDATE user_info SET {} = ? WHERE {} = ? AND {} = ?",
                    std::string("password"),
                    std::string("username"),
                    std::string("email")
          )));
}

void mysql::details::MySQLManagement::roundRobinChecking(std::size_t timeout)
{
          std::unique_lock<std::mutex> _lckg(m_mtx);
          m_ctx.wait(_lckg, [this]() { return !m_queue.empty() || m_stop; });

          if (m_stop) {
                    return;
          }

          for (std::size_t i = 0; i < m_queue.size(); ++i) {
                    mysql::MySQLRAII instance;
                    [[maybe_unused]] bool status = instance->get()->checkTimeout(std::chrono::steady_clock::now(), m_timeout);
                    
                    /*checktimeout error, then create a new connection*/
                    if (!status) [[unlikely]]
                    {
                              /*create a new connection*/
                              m_queue.push(std::move(std::make_unique<mysql::MySQLConnection>(
                                        m_username,
                                        m_password,
                                        m_database,
                                        m_host,
                                        m_port,
                                        this
                              )));

                              m_ctx.notify_one();
                    }
          }
}

mysql::MySQLRAII::MySQLRAII()
          :status(true)
{
          auto optional = details::MySQLManagement::get_instance()->acquire();
          if (!optional.has_value()) {
                    status = false;
          }
          else {
                    m_mysql = std::move(optional.value());
          }
}

mysql::MySQLRAII::~MySQLRAII()
{
          if (status) {
                    details::MySQLManagement::get_instance()->release(std::move(m_mysql));
                    status = false;
          }
}

std::optional<mysql::MySQLRAII::wrapper> mysql::MySQLRAII::operator->()
{
          if (status) {
                    return wrapper(m_mysql.get());
          }
          return std::nullopt;
}