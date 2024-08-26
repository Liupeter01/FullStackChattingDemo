#ifndef _INIREADER_HPP_
#define _INIREADER_HPP_
#include <inicpp.h>
#include <memory>
#include <singleton/singleton.hpp>

struct ServerConfig : public Singleton<ServerConfig> {
  friend class Singleton<ServerConfig>;

public:
  ~ServerConfig() = default;
  unsigned short GateServerPort;

  std::string VerificationServerAddress;

  std::string MySQL_host;
  std::string MySQL_port;
  std::string MySQL_username;
  std::string MySQL_passwd;
  std::string MySQL_database;
  std::size_t MySQL_timeout;

  std::string Redis_ip_addr;
  unsigned short Redis_port;
  std::string Redis_passwd;

  std::string ChattingServiceAddress;
  std::string ChattingServicePort;

private:
  ServerConfig();

  void loadGateServerInfo();
  void loadVerificationServerInfo();
  void loadMySQLInfo();
  void loadRedisInfo();
  void loadChattingServerInfo();

private:
  ini::IniFile m_ini;
};

#endif
