#include <spdlog/spdlog.h>
#include<spdlog/fmt/fmt.h>
#include <boost/asio/ip/tcp.hpp>
#include <boost/mysql/results.hpp>
#include <boost/mysql/statement.hpp>
#include <boost/mysql/row_view.hpp>
#include <service/IOServicePool.hpp>
#include <sql/MySQLConnection.hpp>
#include <boost/mysql/handshake_params.hpp>

mysql::MySQLConnection::MySQLConnection( std::string_view username,
                                                                                std::string_view password,
                                                                                std::string_view database,
                                                                                std::string_view host,
                                                                                std::string_view port) noexcept

          : ctx(IOServicePool::get_instance()->getIOServiceContext())
          , ssl_ctx(boost::asio::ssl::context::tls_client)
          , conn(ctx.get_executor(), ssl_ctx)
          , last_operation_time(std::chrono::steady_clock::now())     /*get operation time*/
{
          try
          {
                    registerSQLStatement();

                    // Resolve the hostname to get a collection of endpoints
                    boost::asio::ip::tcp::resolver resolver(ctx.get_executor());
                    auto endpoints = resolver.resolve(host, port);

                    conn.connect(*endpoints.begin(), boost::mysql::handshake_params(
                              username,
                              password,
                              database
                    ));
          }
          catch (const boost::mysql::error_with_diagnostics& err)
          {
                    // Some errors include additional diagnostics, like server-provided error messages.
                    // Security note: diagnostics::server_message may contain user-supplied values (e.g. the
                    // field value that caused the error) and is encoded using to the connection's character set
                    // (UTF-8 by default). Treat is as untrusted input.
                    spdlog::error("MySQL Connect Error: {0}\n Server diagnostics: {1}", 
                              err.what(), 
                              err.get_diagnostics().server_message().data()
                    );

                    std::abort();
          }
}

mysql::MySQLConnection::~MySQLConnection()
{
          conn.close();
}

void mysql::MySQLConnection::registerSQLStatement()
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
}

std::optional<std::size_t> mysql::MySQLConnection::allocateNewUid()
{
          /*get uid number from database*/
          auto uid = executeCommand(MySQLSelection::ACQUIRE_NEW_UID);
          [[maybe_unused]] auto update = executeCommand(MySQLSelection::UPDATE_UID_COUNTER);

          if (!uid.has_value()) {
                    return std::nullopt;
          }
          
          /*return uid number*/
          boost::mysql::results result = uid.value();
          boost::mysql::row_view row = *result.rows().begin();
          return  static_cast<std::size_t>(row.at(0).as_int64());
}

bool mysql::MySQLConnection::checkAccountAvailability(std::string_view username, std::string_view email)
{
          auto res = executeCommand(MySQLSelection::FIND_EXISTING_USER, username, email);
          if (!res.has_value()) {
                    return false;
          }
          return !res.value().size();
}

bool mysql::MySQLConnection::insertNewUser(MySQLRequestStruct&& request)
{
          std::optional<std::size_t> uid = allocateNewUid();
          if (!uid.has_value()) {
                    return false;
          }
          [[maybe_unused]] auto res = executeCommand(MySQLSelection::CREATE_NEW_USER,
                    request.m_username,
                    request.m_password,
                    std::to_string(uid.value()),
                    request.m_email
          );
          return true;
}

bool mysql::MySQLConnection::registerNewUser(MySQLRequestStruct&& request)
{
          /*check is there anyone who use this username before*/
          if (!checkAccountAvailability(request.m_username, request.m_email)) {
                    return insertNewUser(std::forward<MySQLRequestStruct>(request));
          }
          return false;
}

bool mysql::MySQLConnection::checkTimeout(const std::chrono::steady_clock::time_point& curr, std::size_t timeout)
{
          if (std::chrono::duration_cast<std::chrono::seconds>(curr - last_operation_time).count() > timeout) {
                    return sendHeartBeat();
          }
          return true;
}

bool mysql::MySQLConnection::sendHeartBeat()
{
          return executeCommand(MySQLSelection::HEART_BEAT).has_value();
}

void mysql::MySQLConnection::updateTimer()
{
          last_operation_time = std::chrono::steady_clock::now();
}