#ifndef _INIREADER_HPP_
#define _INIREADER_HPP_
#include <inicpp.h>
#include <singleton/singleton.hpp>

struct ServerConfig : public Singleton<ServerConfig> {
  friend class Singleton<ServerConfig>;

public:
  unsigned short ChattingServerPort;
  std::size_t ChattingServerQueueSize;

  std::string BalanceServiceAddress;
  std::string BalanceServicePort;

  std::string MySQL_host;
  std::string MySQL_port;
  std::string MySQL_username;
  std::string MySQL_passwd;
  std::string MySQL_database;
  std::size_t MySQL_timeout;

  ~ServerConfig() = default;

private:
  ServerConfig() {
    /*init config*/
    m_ini.load(CONFIG_HOME "config.ini");
    loadChattingServiceInfo();
    loadBalanceServiceInfo();
    loadMySQLInfo();
  }

  void loadChattingServiceInfo() {
    ChattingServerPort = m_ini["ChattingServer"]["port"].as<unsigned short>();
    ChattingServerQueueSize =
        m_ini["ChattingServer"]["send_queue_size"].as<int>();
  }

  void loadBalanceServiceInfo() {
    BalanceServiceAddress = m_ini["BalanceService"]["host"].as<std::string>();
    BalanceServicePort =
        std::to_string(m_ini["BalanceService"]["port"].as<unsigned short>());
  }
  void loadMySQLInfo() {
            MySQL_username = m_ini["MySQL"]["username"].as<std::string>();
            MySQL_passwd = m_ini["MySQL"]["password"].as<std::string>();
            MySQL_database = m_ini["MySQL"]["database"].as<std::string>();
            MySQL_host = m_ini["MySQL"]["host"].as<std::string>();
            MySQL_port = m_ini["MySQL"]["port"].as<std::string>();
            MySQL_timeout = m_ini["MySQL"]["timeout"].as<unsigned long>();
  }

private:
  ini::IniFile m_ini;
};

#endif
