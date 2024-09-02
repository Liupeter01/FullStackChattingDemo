#include <server/AsyncServer.hpp>
#include <service/IOServicePool.hpp>
#include <spdlog/spdlog.h>

AsyncServer::AsyncServer(boost::asio::io_context &_ioc, unsigned short port)
    : m_ioc(_ioc),
      m_acceptor(_ioc, boost::asio::ip::tcp::endpoint(
                           boost::asio::ip::address_v4::any(), port)) {
  spdlog::info("Chattintg Server activated, listen on port {}", port);
}

AsyncServer::~AsyncServer() {
  spdlog::critical("Chatting Sever Shutting Down!");
}

void AsyncServer::startAccept() {
  auto &ioc = IOServicePool::get_instance()->getIOServiceContext();
  std::shared_ptr<Session> session = std::make_shared<Session>(ioc, this);

  m_acceptor.async_accept(
      session->s_socket,
      std::bind(&AsyncServer::handleAccept, this, session,
                std::placeholders::_1) /*extend the life length of the session*/
  );
}

void AsyncServer::handleAccept(std::shared_ptr<Session> session,
                               boost::system::error_code ec) {
  if (!ec) {
    /*start session read and write function*/
    session->startSession();

    std::lock_guard<std::mutex> _lckg(m_mtx);
    m_sessions.insert(std::make_pair(session->s_uuid, session));
  } else {
    spdlog::info("Chatting Server Accept {} failed", session->s_uuid);
    this->terminateConnection(session->s_uuid);
  }
  this->startAccept();
}

void AsyncServer::terminateConnection(const std::string &uuid) {
  std::lock_guard<std::mutex> _lckg(m_mtx);

  auto it = this->m_sessions.find(uuid);

  /*add safety consideration*/
  if (it != this->m_sessions.end()) {
    /*shutdown connection*/
    it->second->closeSession();

    /*erase it from map*/
    this->m_sessions.erase(uuid);
  }
}
