#include<http/HttpConnection.hpp>
#include<handler/HandleMethod.hpp>

HTTPConnection::HTTPConnection(boost::asio::ip::tcp::socket &_socket)
          :http_socket(_socket)
{
}

void HTTPConnection::start_service()
{
          activate_receiver();
          check_timeout();
}

void HTTPConnection::check_timeout()
{
          /*extended HTTPConnection class life time*/
          std::shared_ptr<HTTPConnection> extended_lifetime = shared_from_this();

          http_timer.async_wait([extended_lifetime, this](boost::system::error_code ec) {
                    if (!ec) {
                              /*if timeout then shutdown connection*/
                              extended_lifetime->http_socket.close();
                    }
          });
}

void HTTPConnection::activate_receiver()
{
          /*extended HTTPConnection class life time*/
          std::shared_ptr<HTTPConnection> extended_lifetime = shared_from_this();

          boost::beast::http::async_read(http_socket, http_buffer, http_request,
                    [this, extended_lifetime](boost::system::error_code ec, std::size_t bytes_transferred) {
                              boost::ignore_unused(bytes_transferred);
                              if (!ec) {          /*no error occured!*/
                                        process_request();
                              }
                    }
          );
}

void HTTPConnection::process_request()
{
          /*short connection*/
          http_response.keep_alive(false);
          http_response.version(http_request.version());

          /*extended HTTPConnection class life time*/
          std::shared_ptr<HTTPConnection> extended_lifetime = shared_from_this();

          /*HTTP GET method*/
          if (http_request.method() == boost::beast::http::verb::get) {
                    bool successful = HandleMethod::get_instance()->handleGetMethod(http_request.target(), extended_lifetime);
                    if (!successful) 
                    {
                              http_response.result( boost::beast::http::status::not_found);         //Set Response Status
                              http_response.set(boost::beast::http::field::content_type, "text/plain");
                              boost::beast::ostream(http_response.body()) << "404 Not Found!";
                    }
                    else 
                    {
                              http_response.result(boost::beast::http::status::ok);
                              http_response.set(boost::beast::http::field::server, "Beast GateServer");
                    }
                    write_response();
          }
}

void HTTPConnection::write_response()
{
          /*Set Http Body length*/
          http_response.content_length(http_response.body().size());

          /*extended HTTPConnection class life time*/
          std::shared_ptr<HTTPConnection> extended_lifetime = shared_from_this();

          boost::beast::http::async_write(http_socket, http_response,
                    [this, extended_lifetime](boost::beast::error_code ec, std::size_t bytes_transferred) {
                              extended_lifetime->http_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);

                              /*because http has already been sent, so cancel timer*/
                              extended_lifetime->http_timer.cancel();
                    }
          );
}