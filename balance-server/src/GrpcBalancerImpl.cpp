#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <config/ServerConfig.hpp>
#include <grpc/GrpcBalancerImpl.hpp>
#include <network/def.hpp>
#include <redis/RedisManager.hpp>

grpc::GrpcBalancerImpl::UserInfo::UserInfo(
    std::string &&tokens,
    const grpc::GrpcBalancerImpl::ChattingServerConfig &config)
    : m_tokens(std::move(tokens)), m_host(config._host), m_port(config._port) {}

grpc::GrpcBalancerImpl::GrpcBalancerImpl() {
  spdlog::info("Loading {} ChattingServer Configrations",
               ServerConfig::get_instance()->ChattingServerConfig.size());
  for (const auto &server :
       ServerConfig::get_instance()->ChattingServerConfig) {
    spdlog::info("[Loading {}]: {}:{}", server._name, server._host,
                 server._port);
    ChattingServerConfig config;
    config._host = server._host;
    config._port = server._port;
    config._name = server._name;
    config._connections = server._connections;
    servers[server._name] = std::move(config);
  }
}

grpc::GrpcBalancerImpl::~GrpcBalancerImpl() {}

const grpc::GrpcBalancerImpl::ChattingServerConfig &
grpc::GrpcBalancerImpl::serverLoadBalancer() {
          std::lock_guard<std::mutex> _lckg(server_mtx);

          /*remember the lowest load server in iterator*/
          decltype(servers)::iterator min_server = servers.begin();

          connection::ConnectionRAII<redis::RedisConnectionPool, redis::RedisContext> raii;

          /*find key = login and field = server_name in redis, HGET*/
          std::optional<std::string> counter = raii->get()->getValueFromHash(redis_server_login, min_server->first);

          /*
           * if redis doesn't have this key&field in DB, then set the max value
           * or retrieve the counter number from Mem DB
           */
          min_server->second._connections = !counter.has_value() ? INT_MAX : std::stoi(counter.value());

          /*for loop all the servers(including peer server)*/
          for (auto server = servers.begin(); server != servers.end(); ++server) {

                    /*ignore current */
                    if (server->first != min_server->first) {
                              std::optional<std::string> counter = raii->get()->getValueFromHash(redis_server_login, min_server->first);
                              
                              /* 
                               * if redis doesn't have this key&field in DB, then set the max value
                               * or retrieve the counter number from Mem DB
                               */
                              server->second._connections = !counter.has_value() ? INT_MAX : std::stoi(counter.value());

                              if (server->second._connections < min_server->second._connections) {
                                        min_server = server;
                              }
                    }
          }
          return min_server->second;
}

std::optional<std::string>
grpc::GrpcBalancerImpl::getUserToken(std::size_t uuid) {

          connection::ConnectionRAII<redis::RedisConnectionPool, redis::RedisContext> raii;

          /*find key = token_predix + uuid in redis, GET*/
          std::optional<std::string> counter = raii->get()->checkValue(token_prefix + std::to_string(uuid));

          if (!counter.has_value()) {
                    return std::nullopt;
          }
          return counter.value();
}

ServiceStatus
grpc::GrpcBalancerImpl::verifyUserToken(std::size_t uuid,
                                        const std::string &tokens) {
  auto target = getUserToken(uuid);
  if (!target.has_value()) {
    return ServiceStatus::LOGIN_UNSUCCESSFUL;
  }

  return (target.value() == tokens ? ServiceStatus::SERVICE_SUCCESS
                                   : ServiceStatus::LOGIN_INFO_ERROR);
}

void grpc::GrpcBalancerImpl::registerUserInfo(
    std::size_t uuid, std::string &&tokens,
    const grpc::GrpcBalancerImpl::ChattingServerConfig &server) {
          connection::ConnectionRAII<redis::RedisConnectionPool, redis::RedisContext> raii;

          /*find key = token_predix + uuid in redis, GET*/
          if (!raii->get()->setValue(token_prefix + std::to_string(uuid), tokens)) {}
}

::grpc::Status grpc::GrpcBalancerImpl::AddNewUserToServer(
    ::grpc::ServerContext *context,
    const ::message::RegisterToBalancer *request,
    ::message::GetAllocatedChattingServer *response) {

  auto uuid = request->uuid();
  std::optional<std::string> exists = getUserToken(uuid);

  /*get the lowest load server*/
  auto target = serverLoadBalancer();
  response->set_error(
            static_cast<std::size_t>(ServiceStatus::SERVICE_SUCCESS));
  response->set_host(target._host);
  response->set_port(target._port);

  /*check if it is registered?*/
  if (!exists.has_value()) {

    /*generate a user token and store it into the structure first*/
    std::string token = userTokenGenerator();
    response->set_token(token);
    registerUserInfo(uuid, std::move(token), target);
  }
  else {
            response->set_token(exists.value());
  }
  return grpc::Status::OK;
}

::grpc::Status grpc::GrpcBalancerImpl::UserLoginToServer(
    ::grpc::ServerContext *context,
    const ::message::LoginChattingServer *request,
    ::message::LoginChattingResponse *response) {
  /*verify user token*/
  response->set_error(static_cast<std::size_t>(
      verifyUserToken(request->uuid(), request->token())));
  return grpc::Status::OK;
}

::grpc::Status grpc::GrpcBalancerImpl::GetPeerServerInfo(
    ::grpc::ServerContext *context,
    const ::message::GetChattingSeverPeerListsRequest *request,
    ::message::PeerResponse *response) {

  auto target = this->servers.find(request->cur_server_name());

  /*we didn't find cur_server_name in unordered_map*/
  if (target == this->servers.end()) {
    response->set_error(
        static_cast<std::size_t>(ServiceStatus::CHATTING_SERVER_NOT_EXISTS));
  } else {
    std::for_each(
        servers.begin(), servers.end(),
        [&request, &response](const decltype(*servers.begin()) &peer) {
          if (peer.first != request->cur_server_name()) {
            auto new_peer = response->add_lists();
            new_peer->set_name(peer.second._name);
            new_peer->set_host(peer.second._host);
            new_peer->set_port(peer.second._port);
          }
        });

    response->set_error(
        static_cast<std::size_t>(ServiceStatus::SERVICE_SUCCESS));
  }
  return grpc::Status::OK;
}

std::string grpc::GrpcBalancerImpl::userTokenGenerator() {
  boost::uuids::uuid uuid_gen = boost::uuids::random_generator()();
  return boost::uuids::to_string(uuid_gen);
}
