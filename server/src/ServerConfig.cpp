#include<config/ServerConfig.hpp>

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