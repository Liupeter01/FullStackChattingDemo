#pragma once
#ifndef _DEF_HPP_
#include<stdint.h>

enum class ServiceType : uint8_t {
          SERVICE_VERIFICATION,   //get verification code
          SERVICE_REGISTERATION   //user registeration
};

enum class ServiceStatus : uint8_t {
          SERVICE_SUCCESS,        //SUCCESS
          JSONPARSE_ERROR,        //json parsing error
          NETWORK_ERROR           //network failed
};

#define _DEF_HPP_
#endif 