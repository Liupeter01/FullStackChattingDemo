#include <config/ServerConfig.hpp>

ServerConfig::ServerConfig() {
  m_ini.load(CONFIG_HOME "config.ini");
  loadGateServerInfo();
  loadVerificationServerInfo();
  loadMySQLInfo();
  loadRedisInfo();
}

void ServerConfig::loadGateServerInfo() {
  GateServerPort = m_ini["GateServer"]["port"].as<unsigned short>();
}

void ServerConfig::loadVerificationServerInfo() {
  VerificationServerAddress =
      m_ini["VerificationServer"]["host"].as<std::string>() + ':' +
      std::to_string(m_ini["VerificationServer"]["port"].as<unsigned short>());
}

void ServerConfig::loadMySQLInfo() {
  MySQL_username = m_ini["MySQL"]["username"].as<std::string>();
  MySQL_passwd = m_ini["MySQL"]["password"].as<std::string>();
  MySQL_database = m_ini["MySQL"]["database"].as<std::string>();
  MySQL_host = m_ini["MySQL"]["host"].as<std::string>();
  MySQL_port = m_ini["MySQL"]["port"].as<std::string>();
  MySQL_timeout = m_ini["MySQL"]["timeout"].as<unsigned long>();
}

void ServerConfig::loadRedisInfo() {
  Redis_port = m_ini["Redis"]["port"].as<unsigned short>();
  Redis_ip_addr = m_ini["Redis"]["host"].as<std::string>();
  Redis_passwd = m_ini["Redis"]["password"].as<std::string>();
}
