#pragma once
#ifndef _SESSION_HPP_
#define _SESSION_HPP_
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <memory>
#include <string>

template <typename _Base>
struct Session : public std::enable_shared_from_this<Session<_Base>> {
  Session(boost::asio::io_context &_ioc, _Base *my_gate)
      : s_closed(false), s_socket(_ioc), s_gate(my_gate) {
    /*generate uuid string*/
    boost::uuids::uuid uuid_gen = boost::uuids::random_generator()();
    this->s_uuid = boost::uuids::to_string(uuid_gen);
  }

  ~Session() { 
            if (!s_closed) {
                      closeSession();
            }
  }
  void closeSession() {
            /*close by accident or gateserver*/
            s_closed = true;
            s_socket.close();
  }

  bool s_closed;
  std::string s_uuid;
  boost::asio::ip::tcp::socket s_socket;
  _Base *s_gate;
};

#endif
