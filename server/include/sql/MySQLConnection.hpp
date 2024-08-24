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
		  namespace details 
		  {
					class MySQLManagement;
		  }

		  enum class MySQLSelection :uint8_t 
		  {
					HEART_BEAT,
					FIND_EXISTING_USER,
					CREATE_NEW_USER,			 //register new user
					ACQUIRE_NEW_UID,			 //get uid for user
					UPDATE_UID_COUNTER,	//add up to uid accounter
					UPDATE_USER_PASSWD	//update user password
		  };

		  class MySQLConnection
		  {
					friend class details::MySQLManagement;
					MySQLConnection(const MySQLConnection&) = delete;
					MySQLConnection& operator=(const MySQLConnection&) = delete;

		  public:
					MySQLConnection(std::string_view username,
												     std::string_view password,
												     std::string_view database,
												     std::string_view host,
												     std::string_view port,
							                         mysql::details::MySQLManagement* shared) noexcept;

					~MySQLConnection();

		  public:
					bool registerNewUser(MySQLRequestStruct&& request, std::size_t& uuid);
					bool alterUserPassword(MySQLRequestStruct&& request);					
					
					/*is username and email were occupied!*/
					bool checkAccountAvailability(std::string_view username, std::string_view email);
					
					bool checkTimeout(const std::chrono::steady_clock::time_point& curr, std::size_t timeout);

		  private:		
					template<typename ...Args>
					std::optional<boost::mysql::results> executeCommand(MySQLSelection select, Args&&... args);

					void updateTimer();

					/*send heart packet to mysql to prevent from disconnecting*/
					bool sendHeartBeat();

					/*get new uid for user registeration*/
					std::optional<std::size_t> allocateNewUid();

					/*insert new user, call MySQLSelection::CREATE_NEW_USER*/
					bool insertNewUser(MySQLRequestStruct&& request, std::size_t& uuid);

		  private:
					std::shared_ptr<mysql::details::MySQLManagement> m_delegator;

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