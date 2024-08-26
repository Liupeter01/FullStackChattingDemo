#ifndef _INIREADER_HPP_
#define _INIREADER_HPP_
#include <inicpp.h>
#include <memory>
#include <singleton/singleton.hpp>

struct ServerConfig : public Singleton<ServerConfig> {
  friend class Singleton<ServerConfig>;

public:
  ~ServerConfig() = default;
  std::string BalanceServiceAddress;
  std::string BalanceServicePort;

private:
  ServerConfig() {
            m_ini.load(CONFIG_HOME "config.ini");
            loadBalanceServiceInfo();
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
