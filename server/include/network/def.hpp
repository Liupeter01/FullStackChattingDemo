#pragma once
#ifndef _DEF_HPP_
#include<stdint.h>

enum class ServiceType : uint8_t {
          SERVICE_VERIFICATION,   //get verification code
          SERVICE_REGISTERATION   //user registeration
};

enum class ServiceStatus : uint8_t {
          SERVICE_SUCCESS,                              //SUCCESS
          JSONPARSE_ERROR,                             //json parsing error
          NETWORK_ERROR,                               //network failed
          GRPC_ERROR,                                        //grpc error
          REDIS_UNKOWN_ERROR,                   //redis server error code
          REDIS_CPATCHA_NOT_FOUND,         //redis no cpatcha
          MYSQL_USERNAME_WAS_TAKEN    //mysql username have already been taken
};

#define _DEF_HPP_
#endif 