#include<spdlog/spdlog.h>
#include<server/GateServer.hpp>
#include<http/HttpConnection.hpp>
#include<service/IOServicePool.hpp>

GateServer::GateServer(boost::asio::io_context& _ioc, unsigned short port)
          :m_ioc(_ioc)
          , m_acceptor(_ioc, boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port))
{
          spdlog::info("Server activated, listen on port {}", port);
          this->serverStart();
}

void GateServer::serverStart()
{
          boost::asio::io_context &ioc = IOServicePool::get_instance()->getIOServiceContext();   //get ioc
          std::shared_ptr<Session> session = std::make_shared<Session>(ioc,  this);

          this->m_acceptor.async_accept(
                    session->s_socket,
                    std::bind(&GateServer::handleAccept, this, session, std::placeholders::_1)
          );
}

void GateServer::handleAccept(std::shared_ptr<Session> session, boost::system::error_code ec)
{
          if (!ec) 
          {
                    /*add to session pool*/
                    this->m_sessions.insert(std::make_pair(session->s_uuid, session));
          
                    /*establish HTTPConnection to handle socket*/
                    std::shared_ptr<HTTPConnection> http(std::make_shared<HTTPConnection>(session->s_socket));
                    http->start_service();
          }
          else   /*error occured!*/
          {
                    this->terminateSession(session->s_uuid);
          }

          /*accept connection recursively*/
          this->serverStart();
}

void GateServer::terminateSession(const std::string& uuid)
{
          this->m_sessions.erase(uuid);
}