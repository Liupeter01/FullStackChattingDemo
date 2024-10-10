#ifndef _INIREADER_HPP_
#define _INIREADER_HPP_
#include <inicpp.h>
#include <memory>
#include <singleton/singleton.hpp>
#include <spdlog/spdlog.h>
#include <vector>

struct ChattingServer {
  std::string _host;
  std::string _port;
  std::string _name;
  std::size_t _connections;
};

struct ServerConfig : public Singleton<ServerConfig> {
  friend class Singleton<ServerConfig>;

public:
  ~ServerConfig() = default;

  std::string Redis_ip_addr;
  unsigned short Redis_port;
  std::string Redis_passwd;

  std::string BalanceServiceAddress;
  std::string BalanceServicePort;
  std::vector<ChattingServer> ChattingServerConfig;

private:
  ServerConfig() {
    /*init config*/
    ChattingServerConfig.resize(0);
    m_ini.load(CONFIG_HOME "config.ini");
    loadBalanceServiceInfo();
    loadChattingServiceInfo();
  }

  void loadRedisInfo() {
    Redis_port = m_ini["Redis"]["port"].as<unsigned short>();
    Redis_ip_addr = m_ini["Redis"]["host"].as<std::string>();
    Redis_passwd = m_ini["Redis"]["password"].as<std::string>();
  }

  void loadBalanceServiceInfo() {
    BalanceServiceAddress = m_ini["BalanceService"]["host"].as<std::string>();
    BalanceServicePort =
        std::to_string(m_ini["BalanceService"]["port"].as<unsigned short>());
  }

  void loadChattingServiceInfo() {
    std::size_t ammount = m_ini["ChattingServer"]["number"].as<int>();

    /*prevent invalid number*/
    if (static_cast<int>(ammount) <= 0) {
      return;
    }
    for (std::size_t start = 0; start < ammount; start++) {
      ChattingServer server;
      server._name = fmt::format("ChattingServer{}", std::to_string(start));
      server._host = m_ini[server._name]["host"].as<std::string>();
      server._port = m_ini[server._name]["port"].as<std::string>();
      server._connections = 0;
      ChattingServerConfig.push_back(std::move(server));
    }
  }

private:
  ini::IniFile m_ini;
};

#endif
