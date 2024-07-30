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
                    boost::beast::ostream(conn->http_response.body()) << "get_test";
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