#pragma once
#ifndef _MYSQLCONNECTION_HPP_
#define _MYSQLCONNECTION_HPP_
#include <map>
#include <chrono>
#include<optional>
#include <string_view>
#include <boost/mysql/tcp_ssl.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>

struct MySQLRequestStruct
{
		  std::string_view m_username;
		  std::string_view m_password;
		  std::string_view m_email;
};

namespace mysql
{
		  class MySQLManagement;

		  enum class MySQLSelection :uint8_t 
		  {
					HEART_BEAT,
					FIND_EXISTING_USER,
					CREATE_NEW_USER,
					ACQUIRE_NEW_UID,
					UPDATE_UID_COUNTER
		  };

		  class MySQLConnection
		  {
					friend class MySQLManagement;
					MySQLConnection(const MySQLConnection&) = delete;
					MySQLConnection& operator=(const MySQLConnection&) = delete;

		  public:
					MySQLConnection(std::string_view username,
												     std::string_view password,
												     std::string_view database,
												     std::string_view host,
												     std::string_view port) noexcept;

					~MySQLConnection();

		  public:
					bool registerNewUser(MySQLRequestStruct&& request);
					bool checkTimeout(const std::chrono::steady_clock::time_point& curr, std::size_t timeout);

		  private:		
					void registerSQLStatement();

					template<typename ...Args>
					std::optional<boost::mysql::results> executeCommand(MySQLSelection select, Args&&... args) {
							  try {
										boost::mysql::results result;
										std::string key = m_sql[select];
										spdlog::info("Executing MySQL Query: {}", key);
										boost::mysql::statement stmt = conn.prepare_statement(key);
										conn.execute(stmt.bind(std::forward<Args>(args)...), result);
										return result;
							  }
							  catch (const boost::mysql::error_with_diagnostics& err)
							  {
										spdlog::error("{0}:{1} Operation failed with error code: {2} Server diagnostics: {3}",
												  __FILE__,
												  __LINE__,
												  std::to_string(err.code().value()),
												  err.get_diagnostics().server_message().data()
										);
										return std::nullopt;
							  }
					}

					void updateTimer();

					/*send heart packet to mysql to prevent from disconnecting*/
					bool sendHeartBeat();

					/*is username and email were occupied!*/
					bool checkAccountAvailability(std::string_view username, std::string_view email);

					/*get new uid for user registeration*/
					std::optional<std::size_t> allocateNewUid();

					/*insert new user, call MySQLSelection::CREATE_NEW_USER*/
					bool insertNewUser(MySQLRequestStruct&& request);

		  private:
					std::map<MySQLSelection, std::string> m_sql;

					// The execution context, required to run I/O operations.
					boost::asio::io_context &ctx;

					// The SSL context, required to establish TLS connections.
					// The default SSL options are good enough for us at this point.
					boost::asio::ssl::context ssl_ctx;

					// Represents a connection to the MySQL server.
					boost::mysql::tcp_ssl_connection conn;

					/*last operation time*/
					std::chrono::steady_clock::time_point last_operation_time;
		  };
}

#endif // !_MYSQLCONNECTION_HPP_