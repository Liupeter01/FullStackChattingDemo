#pragma once
#ifndef _GATESERVER_HPP_
#define _GATESERVER_HPP_
#include <map>
#include <server/session.hpp>

class GateServer : public std::enable_shared_from_this<GateServer> {
public:
  GateServer(boost::asio::io_context &_ioc, unsigned short port);
  ~GateServer() = default;

public:
  void serverStart();

private:
  void handleAccept(std::shared_ptr<Session> session,
                    boost::system::error_code ec);
  void terminateSession(const std::string &uuid);

private:
  boost::asio::io_context &m_ioc;
  boost::asio::ip::tcp::acceptor m_acceptor;
  std::map<
      /*uuid            */ std::string,
      /*connections*/ std::shared_ptr<Session>>
      m_sessions;
};

#endif // !_GATESERVER_HPP_