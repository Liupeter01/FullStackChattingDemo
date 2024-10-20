#pragma once
#ifndef _MYSQLCONNECTION_HPP_
#define _MYSQLCONNECTION_HPP_
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/mysql/tcp_ssl.hpp>
#include <chrono>
#include <map>
#include <optional>
#include <server/UserNameCard.hpp>
#include <string_view>

/*delcaration*/
struct UserNameCard;

struct MySQLRequestStruct {
  std::string_view m_username;
  std::string_view m_password;
  std::string_view m_email;
};

namespace mysql {
class MySQLConnectionPool;

enum class MySQLSelection : uint8_t {
  HEART_BEAT,
  FIND_EXISTING_USER,
  CREATE_NEW_USER,    // register new user
  ACQUIRE_NEW_UID,    // get uid for user
  UPDATE_UID_COUNTER, // add up to uid accounter
  UPDATE_USER_PASSWD, // update user password
  USER_LOGIN_CHECK,   // check login username & password
  USER_UUID_CHECK,    // check account uuid in DB
  USER_PROFILE,        // check account user profile
  GET_USER_UUID,     //get uuid by username
  USER_FRIEND_REQUEST      //User A send friend request to B
};

class MySQLConnection {
  friend class MySQLConnectionPool;
  MySQLConnection(const MySQLConnection &) = delete;
  MySQLConnection &operator=(const MySQLConnection &) = delete;

public:
  MySQLConnection(std::string_view username, std::string_view password,
                  std::string_view database, std::string_view host,
                  std::string_view port,
                  mysql::MySQLConnectionPool *shared) noexcept;

  ~MySQLConnection();

public:
  /*get user profile*/
  std::optional<std::unique_ptr<UserNameCard>> getUserProfile(std::size_t uuid);

  /*create user friend request MySQLSelection::USER_FRIEND_REQUEST*/
  bool createFriendRequest(const std::size_t src_uuid, const std::size_t dst_uuid, std::string_view nickname, std::string_view message);

  /*insert new user, call MySQLSelection::CREATE_NEW_USER*/
  bool registerNewUser(MySQLRequestStruct &&request);
  bool alterUserPassword(MySQLRequestStruct &&request);

  /*login username & password check*/
  std::optional<std::size_t> checkAccountLogin(std::string_view username,
                                               std::string_view password);

  /*is username and email were occupied!*/
  bool checkAccountAvailability(std::string_view username,
                                std::string_view email);

  bool checkTimeout(const std::chrono::steady_clock::time_point &curr,
                    std::size_t timeout);

  bool checkUUID(std::size_t uuid);

  std::optional<std::size_t> getUUIDByUsername(std::string_view username);
  std::optional<std::string> getUsernameByUUID(std::size_t uuid);

private:
  template <typename... Args>
  std::optional<boost::mysql::results> executeCommand(MySQLSelection select,
                                                      Args &&...args);

  void updateTimer();

  /*send heart packet to mysql to prevent from disconnecting*/
  bool sendHeartBeat();

private:
  std::shared_ptr<MySQLConnectionPool> m_delegator;

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
} // namespace mysql

#endif // !_MYSQLCONNECTION_HPP_
