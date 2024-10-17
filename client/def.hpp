#pragma once
#ifndef _DEF_HPP_
#include <stdint.h>

enum class ServiceType : uint8_t {
  SERVICE_VERIFICATION,  // get verification code
  SERVICE_REGISTERATION, // user registeration
  SERVICE_CHECKEEXISTS,  // check account existance
  SERVICE_RESETPASSWD,   // password reset

  /*
   * before login, user has to get the address of
   * chatting server which is dispatched by balance-server
   */
  SERVICE_LOGINDISPATCH,
  SERVICE_LOGINSERVER,            // try to login into server
  SERVICE_LOGINRESPONSE,          // login server response
  SERVICE_SEARCHUSERNAME,         // client search another user's username
  SERVICE_SEARCHUSERNAMERESPONSE, //
  SERVICE_RESERVE_4,              //
  SERVICE_UNKNOWN                 // unkown service
};

enum class ServiceStatus : uint8_t {
  SERVICE_SUCCESS,            // SUCCESS
  JSONPARSE_ERROR,            // json parsing error
  NETWORK_ERROR,              // network failed
  GRPC_ERROR,                 // grpc error
  GRPC_SERVER_NOT_EXISTS,     // GRPC server not exist
  GRPC_SERVER_ALREADY_EXISTS, // GRPC server already exists
  CHATTING_SERVER_NOT_EXISTS, // chatting server not exist
  REDIS_UNKOWN_ERROR,         // redis server error code
  REDIS_CPATCHA_NOT_FOUND,    // redis no cpatcha
  MYSQL_INTERNAL_ERROR,       // mysql error
  MYSQL_MISSING_INFO,         // mysql missing account info
  MYSQL_ACCOUNT_NOT_EXISTS,   // mysql account not exists
  LOGIN_INFO_ERROR,           // login info error
  LOGIN_UNSUCCESSFUL, // common login uncessfully reason, due to internel error
  LOGIN_FOR_MULTIPLE_TIMES,    // this user have already logined!
  SEARCHING_USERNAME_NOT_FOUND // client search another user's username not
                               // found
};

#define _DEF_HPP_
#endif
