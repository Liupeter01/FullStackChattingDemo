#pragma once
#ifndef _SESSION_HPP_
#define _SESSION_HPP_
#include<string>
#include<memory>
#include<boost/asio.hpp>

/*declare gateserver for delegation*/
class GateServer;

struct Session
          :public std::enable_shared_from_this<Session>
{
          Session(boost::asio::io_context& _ioc, GateServer* my_gate);

          std::string s_uuid; 
          boost::asio::ip::tcp::socket s_socket;
          GateServer* s_gate;
};

#endif