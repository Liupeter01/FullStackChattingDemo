#include <grpc/GrpcVerificationService.hpp>
#include <handler/HandleMethod.hpp>
#include <http/HttpConnection.hpp>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <network/def.hpp> //network errorcode defs
#include <redis/RedisManager.hpp>
#include <spdlog/spdlog.h>
#include <sql/MySQLManagement.hpp>

HandleMethod::~HandleMethod() {}

HandleMethod::HandleMethod() {
  /*register both get and post callbacks*/
  registerCallBacks();
}

void HandleMethod::registerGetCallBacks() {
  this->get_method_callback.emplace(
      "/get_test", [](std::shared_ptr<HTTPConnection> conn) {
        std::size_t counter{0};
        boost::beast::ostream(conn->http_response.body()) << "get_test";
        for (const auto &param : conn->http_params) {
          boost::beast::ostream(conn->http_response.body())
              << "index = " << ++counter << ", key = " << param.first
              << ", value = " << param.second << '\n';
        }
      });
}

void HandleMethod::registerPostCallBacks() {
  this->post_method_callback.emplace(
      "/get_verification",
      [this](std::shared_ptr<HTTPConnection> conn) -> bool {
        conn->http_response.set(boost::beast::http::field::content_type,
                                "text/json");
        auto body =
            boost::beast::buffers_to_string(conn->http_request.body().data());

        spdlog::info("Server receive post data: {}", body.c_str());

        Json::Value send_root; /*write into body*/
        Json::Value src_root;  /*store json from client*/
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

        /*Get email string and send to grpc service*/
        auto email = src_root["email"].asString();

        spdlog::info("Server receive verification request, email addr: {}",
                     email.c_str());

        auto response = gRPCVerificationService::getVerificationCode(email);

        send_root["error"] = response.error();
        send_root["email"] = src_root["email"].asString();
        boost::beast::ostream(conn->http_response.body())
            << send_root.toStyledString();
        return true;
      });

  this->post_method_callback.emplace(
      "/post_registration",
      [this](std::shared_ptr<HTTPConnection> conn) -> bool {
        conn->http_response.set(boost::beast::http::field::content_type,
                                "text/json");
        auto body =
            boost::beast::buffers_to_string(conn->http_request.body().data());

        spdlog::info("Server receive registration request, post data: {}",
                     body.c_str());

        Json::Value send_root; /*write into body*/
        Json::Value src_root;  /*store json from client*/
        Json::Reader reader;

        /*parsing failed*/
        if (!reader.parse(body, src_root)) {
          jsonParsingError(conn);
          return false;
        }

        /*parsing failed*/
        if (!(src_root.isMember("username") && src_root.isMember("password") &&
              src_root.isMember("email") && src_root.isMember("cpatcha"))) {
          jsonParsingError(conn);
          return false;
        }

        /*Get email string and send to grpc service*/
        Json::String username = src_root["username"].asString();
        Json::String password = src_root["password"].asString();
        Json::String email = src_root["email"].asString();
        Json::String cpatcha = src_root["cpatcha"].asString();

        /*find verification code by checking email in redis*/
        redis::RedisRAII raii;
        std::optional<std::string> verification_code =
            raii->get()->checkValue(email);

        /*
         * Redis
         * no verification code found!!
         */
        if (!verification_code.has_value()) {
          redisError(conn);
          return false;
        }

        if (verification_code.value() != cpatcha) {
          captchaError(conn);
          return false;
        }

        MySQLRequestStruct request;
        request.m_username = username;
        request.m_password = password;
        request.m_email = email;

        /*get required uuid, and return it back to user!*/
        std::size_t uuid;

        /*MYSQL(start to create a new user)*/
        mysql::MySQLRAII mysql;
        if (!mysql->get()->registerNewUser(std::move(request), uuid)) {
          registerError(conn);
          return false;
        }

        send_root["error"] =
            static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS);
        send_root["username"] = username;
        send_root["password"] = password;
        send_root["email"] = email;
        send_root["uuid"] = std::to_string(uuid);

        boost::beast::ostream(conn->http_response.body())
            << send_root.toStyledString();
        return true;
      });
}

void HandleMethod::jsonParsingError(std::shared_ptr<HTTPConnection> conn) {
  Json::Value root;
  spdlog::error("Failed to parse json data");

  root["error"] = static_cast<uint8_t>(ServiceStatus::JSONPARSE_ERROR);
  boost::beast::ostream(conn->http_response.body()) << root.toStyledString();
}

void HandleMethod::redisError(std::shared_ptr<HTTPConnection> conn) {
  Json::Value root;

  spdlog::error("Internel redis server error!");

  root["error"] = static_cast<uint8_t>(ServiceStatus::REDIS_UNKOWN_ERROR);
  boost::beast::ostream(conn->http_response.body()) << root.toStyledString();
}

void HandleMethod::captchaError(std::shared_ptr<HTTPConnection> conn) {
  Json::Value root;

  spdlog::error("CPATCHA is different from Redis DB!");

  root["error"] = static_cast<uint8_t>(ServiceStatus::REDIS_CPATCHA_NOT_FOUND);
  boost::beast::ostream(conn->http_response.body()) << root.toStyledString();
}

void HandleMethod::registerError(std::shared_ptr<HTTPConnection> conn) {
  Json::Value root;

  spdlog::error("MySQL internel error!");

  root["error"] = static_cast<uint8_t>(ServiceStatus::MYSQL_INTERNAL_ERROR);
  boost::beast::ostream(conn->http_response.body()) << root.toStyledString();
}

void HandleMethod::registerCallBacks() {
  registerGetCallBacks();
  registerPostCallBacks();
}

bool HandleMethod::handleGetMethod(
    std::string str, std::shared_ptr<HTTPConnection> extended_lifetime) {
  /*Callback Func Not Found*/
  if (get_method_callback.find(str) == get_method_callback.end()) {
    return false;
  }
  get_method_callback[str](extended_lifetime);
  return true;
}

bool HandleMethod::handlePostMethod(
    std::string str, std::shared_ptr<HTTPConnection> extended_lifetime) {
  /*Callback Func Not Found*/
  if (post_method_callback.find(str) == post_method_callback.end()) {
    return false;
  }
  [[maybe_unused]] bool res = post_method_callback[str](extended_lifetime);
  return true;
}
