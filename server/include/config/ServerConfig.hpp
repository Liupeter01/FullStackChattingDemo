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
          std::string MySQLAddress;
          std::string MySQLPasswd;
          std::string RedisAddress;
          std::string RedisPasswd;

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