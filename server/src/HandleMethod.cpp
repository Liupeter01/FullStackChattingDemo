#include<network/def.hpp>     //network errorcode defs
#include<json/json.h>
#include<json/value.h>
#include<json/reader.h>
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

void HandleMethod::registerGetCallBacks()
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

void HandleMethod::registerPostCallBacks()
{
          this->post_method_callback.emplace("/get_verification", [this](std::shared_ptr<HTTPConnection> conn)->bool 
          {
                    conn->http_response.set(boost::beast::http::field::content_type, "text/json");
                    auto body = boost::beast::buffers_to_string(conn->http_request.body().data());

#ifdef _DEBUG
                    printf("[NOTICE]: server receive post data: %s\n", body.c_str());
#endif
                    Json::Value send_root;        /*write into body*/
                    Json::Value src_root;         /*store json from client*/
                    Json::Reader reader;

                    /*parsing failed*/
                    if (!reader.parse(body, src_root)) {
                              jsonParsingError(conn);
                              return false;
                    }

                    if (!src_root.isMember("email")) {
                              jsonParsingError(conn);
                              return false;
                    }

                    send_root["error"] = static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS);
                    send_root["email"] = src_root["email"].asString();
                    boost::beast::ostream(conn->http_response.body()) << send_root.toStyledString();
                    return true;
          });
}

void HandleMethod::jsonParsingError(std::shared_ptr<HTTPConnection> conn)
{
          Json::Value root;

#ifdef _DEBUG
          printf("[ERROR]: failed to parse json data!\n");
#endif

          root["error"] = static_cast<uint8_t>(ServiceStatus::JSONPARSE_ERROR);
          boost::beast::ostream(conn->http_response.body()) << root.toStyledString();
}

void HandleMethod::registerCallBacks()
{
          registerGetCallBacks();
          registerPostCallBacks();
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

bool HandleMethod::handlePostMethod(std::string str, std::shared_ptr<HTTPConnection> extended_lifetime)
{
          /*Callback Func Not Found*/
          if (post_method_callback.find(str) == post_method_callback.end()) {
                    return false;
          }
          [[maybe_unused]] bool res = post_method_callback[str](extended_lifetime);
          return true;
}