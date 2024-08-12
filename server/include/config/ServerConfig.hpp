#ifndef _INIREADER_HPP_
#define _INIREADER_HPP_
#include<memory>
#include<inicpp.h>
#include<singleton/singleton.hpp>

struct ServerConfig : public Singleton< ServerConfig>
{
          friend class Singleton< ServerConfig>;

public:
          ~ServerConfig() = default;
          unsigned short GateServerPort;

          std::string VerificationServerAddress;

          std::string MySQL_ip_addr;
          unsigned short MySQL_port;
          std::string MySQL_passwd;

          std::string Redis_ip_addr;
          unsigned short Redis_port;
          std::string Redis_passwd;

private:
          ServerConfig();

          void loadGateServerInfo();
          void loadVerificationServerInfo();
          void loadMySQLInfo();
          void loadRedisInfo();

private:
          ini::IniFile m_ini;
};

#endif