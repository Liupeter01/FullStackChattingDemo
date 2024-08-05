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
          unsigned short VerificationServerPort;

private:
          ServerConfig() {
                    m_ini.load(CONFIG_HOME"config.ini");
                    GateServerPort = m_ini["GateServer"]["port"].as<unsigned short>();
                    VerificationServerPort = m_ini["VerificationServer"]["port"].as<unsigned short>();
          }

private:
          ini::IniFile m_ini;
};

#endif