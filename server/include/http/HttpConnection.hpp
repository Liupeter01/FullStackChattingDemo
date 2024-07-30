#pragma once
#ifndef _HTTPCONNECTION_HPP_
#define _HTTPCONNECTION_HPP_
#include<memory>
#include<boost/asio.hpp>
#include<boost/beast/core.hpp>
#include<boost/beast/http.hpp>
#include<boost/beast/version.hpp>

class HandleMethod;

class HTTPConnection
          :public std::enable_shared_from_this<HTTPConnection> 
{
          friend class HandleMethod;

public:
          HTTPConnection(boost::asio::ip::tcp::socket& _socket);
          ~HTTPConnection() = default;
          void start_service();

private:
          void check_timeout();
          void activate_receiver();
          void process_request();
          void write_response();

private:
          boost::asio::ip::tcp::socket& http_socket;
          boost::beast::flat_buffer http_buffer{ 8192 };
          boost::beast::http::request<boost::beast::http::dynamic_body> http_request;
          boost::beast::http::response<boost::beast::http::dynamic_body> http_response;
          boost::beast::net::steady_timer http_timer{
                    http_socket.get_executor(),   /*io context*/
                    std::chrono::minutes(1)         /*timer setting*/
          };
};
#endif