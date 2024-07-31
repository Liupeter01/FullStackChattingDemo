#include<handler/HandleMethod.hpp>
#include<http/HttpConnection.hpp>

HandleMethod::~HandleMethod()
{
}

HandleMethod::HandleMethod()
{
          /*register both get and post callbacks*/
          registerCallBacks();
}

void HandleMethod::registerCallBacks()
{
          this->get_method_callback.emplace("/get_test", [](std::shared_ptr<HTTPConnection> conn) {
                    std::size_t counter{ 0 };
                    boost::beast::ostream(conn->http_response.body()) << "get_test";
                    for (const auto& param : conn->http_params) {
                              boost::beast::ostream(conn->http_response.body())
                                        << "index = " << ++counter
                                        << ", key = " << param.first
                                        << ", value = " << param.second << '\n';
                    }
         });
}

bool HandleMethod::handleGetMethod(std::string str, std::shared_ptr<HTTPConnection> extended_lifetime)
{
          /*Callback Func Not Found*/
          if (get_method_callback.find(str) == get_method_callback.end()) {
                    return false;       
          }
          get_method_callback[str](extended_lifetime);
          return true;
}