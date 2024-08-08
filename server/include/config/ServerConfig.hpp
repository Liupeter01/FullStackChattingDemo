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

ServerConfig::ServerConfig()
{
          m_ini.load(CONFIG_HOME"config.ini");
          loadGateServerInfo();
          loadVerificationServerInfo();
          loadMySQLInfo();
          loadRedisInfo();
}

void ServerConfig::loadGateServerInfo() 
{
          GateServerPort = m_ini["GateServer"]["port"].as<unsigned short>();
}

void ServerConfig::loadVerificationServerInfo()
{
          VerificationServerAddress =
                    m_ini["VerificationServer"]["host"].as<std::string>() + ':'
                    + std::to_string(m_ini["VerificationServer"]["port"].as<unsigned short>());
}

void ServerConfig::loadMySQLInfo()
{
          // m_ini["MySQL"]["host"].as<std::string>();
          // m_ini["MySQL"]["port"].as<unsigned short>();
          MySQLPasswd = m_ini["MySQL"]["password"].as<std::string>();
}

void ServerConfig::loadRedisInfo()
{
          // m_ini["Redis"]["host"].as<std::string>();
          // m_ini["Redis"]["port"].as<unsigned short>();
          RedisPasswd = m_ini["Redis"]["password"].as<std::string>();
}

#endif