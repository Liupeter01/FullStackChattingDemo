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

  ~ServerConfig() = default;

private:
  ServerConfig() {
    /*init config*/
    m_ini.load(CONFIG_HOME "config.ini");
    loadChattingServiceInfo();
    loadBalanceServiceInfo();
  }

  void loadChattingServiceInfo() {
            ChattingServerPort = m_ini["ChattingServer"]["port"].as<unsigned short>();
            ChattingServerQueueSize = m_ini["ChattingServer"]["send_queue_size"].as<int>();
  }

  void loadBalanceServiceInfo() {
            BalanceServiceAddress = m_ini["BalanceService"]["host"].as<std::string>();
            BalanceServicePort =
                      std::to_string(m_ini["BalanceService"]["port"].as<unsigned short>());
  }

private:
  ini::IniFile m_ini;
};

#endif
