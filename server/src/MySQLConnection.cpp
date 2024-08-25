#include <boost/asio/ip/tcp.hpp>
#include <boost/mysql/handshake_params.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/row_view.hpp>
#include <boost/mysql/statement.hpp>
#include <service/IOServicePool.hpp>
#include <spdlog/fmt/fmt.h>
#include <spdlog/spdlog.h>
#include <sql/MySQLConnectionPool.hpp>

mysql::MySQLConnection::MySQLConnection(
    std::string_view username, std::string_view password,
    std::string_view database, std::string_view host, std::string_view port,
    mysql::MySQLConnectionPool *shared) noexcept

    : ctx(IOServicePool::get_instance()->getIOServiceContext()),
      ssl_ctx(boost::asio::ssl::context::tls_client),
      conn(ctx.get_executor(), ssl_ctx),
      last_operation_time(
          std::chrono::steady_clock::now()) /*get operation time*/
      ,
      m_delegator(std::shared_ptr<mysql::MySQLConnectionPool>(
          shared, [](mysql::MySQLConnectionPool *) {})) {
  try {
    // Resolve the hostname to get a collection of endpoints
    boost::asio::ip::tcp::resolver resolver(ctx.get_executor());
    auto endpoints = resolver.resolve(host, port);

    conn.connect(*endpoints.begin(),
                 boost::mysql::handshake_params(username, password, database));
  } catch (const boost::mysql::error_with_diagnostics &err) {
    // Some errors include additional diagnostics, like server-provided error
    // messages. Security note: diagnostics::server_message may contain
    // user-supplied values (e.g. the field value that caused the error) and is
    // encoded using to the connection's character set (UTF-8 by default). Treat
    // is as untrusted input.
    spdlog::error("MySQL Connect Error: {0}\n Server diagnostics: {1}",
                  err.what(), err.get_diagnostics().server_message().data());

    std::abort();
  }
}

mysql::MySQLConnection::~MySQLConnection() { conn.close(); }

template <typename... Args>
std::optional<boost::mysql::results>
mysql::MySQLConnection::executeCommand(MySQLSelection select, Args &&...args) {
  try {
    boost::mysql::results result;
    std::string key = m_delegator.get()->m_sql[select];
    spdlog::info("Executing MySQL Query: {}", key);
    boost::mysql::statement stmt = conn.prepare_statement(key);
    conn.execute(stmt.bind(std::forward<Args>(args)...), result);
    return result;
  } catch (const boost::mysql::error_with_diagnostics &err) {
    spdlog::error(
        "{0}:{1} Operation failed with error code: {2} Server diagnostics: {3}",
        __FILE__, __LINE__, std::to_string(err.code().value()),
        err.get_diagnostics().server_message().data());
    return std::nullopt;
  }
}

std::optional<std::size_t> mysql::MySQLConnection::allocateNewUid() {
  /*get uid number from database*/
  auto uid = executeCommand(MySQLSelection::ACQUIRE_NEW_UID);
  [[maybe_unused]] auto update =
      executeCommand(MySQLSelection::UPDATE_UID_COUNTER);

  if (!uid.has_value()) {
    return std::nullopt;
  }

  /*return uid number*/
  boost::mysql::results result = uid.value();
  boost::mysql::row_view row = *result.rows().begin();
  return static_cast<std::size_t>(row.at(0).as_int64());
}

std::optional<std::size_t>
mysql::MySQLConnection::checkAccountLogin(std::string_view username,
                                          std::string_view password) {
  auto res =
      executeCommand(MySQLSelection::USER_LOGIN_CHECK, username, password);
  if (!res.has_value()) {
    return false;
  }
  boost::mysql::results result = res.value();
  if (!result.size()) {
    return false;
  }
  boost::mysql::row_view row = *result.rows().begin();
  return static_cast<std::size_t>(row.at(0).as_int64());
}

bool mysql::MySQLConnection::checkAccountAvailability(std::string_view username,
                                                      std::string_view email) {
  auto res =
      executeCommand(MySQLSelection::FIND_EXISTING_USER, username, email);
  if (!res.has_value()) {
    return false;
  }

  boost::mysql::results result = res.value();
  return result.rows().size();
}

bool mysql::MySQLConnection::insertNewUser(MySQLRequestStruct &&request,
                                           std::size_t &uuid) {
  std::optional<std::size_t> uid = allocateNewUid();
  if (!uid.has_value()) {
    return false;
  }

  /*return back to the client!*/
  uuid = uid.value();

  [[maybe_unused]] auto res = executeCommand(
      MySQLSelection::CREATE_NEW_USER, request.m_username, request.m_password,
      std::to_string(uid.value()), request.m_email);
  return true;
}

bool mysql::MySQLConnection::registerNewUser(MySQLRequestStruct &&request,
                                             std::size_t &uuid) {
  /*check is there anyone who use this username before*/
  if (!checkAccountAvailability(request.m_username, request.m_email)) {
    return insertNewUser(std::forward<MySQLRequestStruct>(request), uuid);
  }
  return false;
}

bool mysql::MySQLConnection::alterUserPassword(MySQLRequestStruct &&request) {
  if (!checkAccountAvailability(request.m_username, request.m_email)) {
    return false;
  }

  [[maybe_unused]] auto res =
      executeCommand(MySQLSelection::UPDATE_USER_PASSWD, request.m_password,
                     request.m_username, request.m_email);
  return true;
}

bool mysql::MySQLConnection::checkTimeout(
    const std::chrono::steady_clock::time_point &curr, std::size_t timeout) {
  if (std::chrono::duration_cast<std::chrono::seconds>(curr -
                                                       last_operation_time)
          .count() > timeout) {
    return sendHeartBeat();
  }
  return true;
}

bool mysql::MySQLConnection::sendHeartBeat() {
  return executeCommand(MySQLSelection::HEART_BEAT).has_value();
}

void mysql::MySQLConnection::updateTimer() {
  last_operation_time = std::chrono::steady_clock::now();
}
