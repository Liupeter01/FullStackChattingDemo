#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <config/ServerConfig.hpp>
#include <grpc/GrpcBalancerImpl.hpp>
#include <network/def.hpp>

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

const grpc::ChattingServerConfig &grpc::GrpcBalancerImpl::serverLoadBalancer() {
  std::lock_guard<std::mutex> _lckg(server_mtx);

  /*remember the lowest load server in iterator*/
  decltype(servers)::const_iterator min = servers.begin();
  for (auto ib = servers.begin(); ib != servers.end(); ib++) {
    if (ib->second._connections < min->second._connections) {
      min = ib;
    }
  }
  return min->second;
}

void grpc::GrpcBalancerImpl::registerUserToken(std::size_t uuid,
                                               const std::string &token) {
  std::lock_guard<std::mutex> _lckg(token_mtx);
  tokens[uuid] = token;
}

std::optional<std::string_view>
grpc::GrpcBalancerImpl::getUserToken(std::size_t uuid) {
  auto target = tokens.find(uuid);
  if (target == tokens.end()) {
    return std::nullopt;
  }
  return target->second;
}

ServiceStatus grpc::GrpcBalancerImpl::verifyUserToken(std::size_t uuid, const std::string& tokens)
{
          std::optional<std::string_view> target = getUserToken(uuid);
          if (!target.has_value()) {
                    return ServiceStatus::LOGIN_UNSUCCESSFUL;
          }
          return (target.value() == tokens ? ServiceStatus::SERVICE_SUCCESS : ServiceStatus::LOGIN_INFO_ERROR);
}

::grpc::Status grpc::GrpcBalancerImpl::AddNewUserToServer(
    ::grpc::ServerContext *context,
    const ::message::RegisterToBalancer *request,
    ::message::GetAllocatedChattingServer *response) {
  /*get the lowest load server*/
  auto target = serverLoadBalancer();

  /*generate a user token and store it into the structure first*/
  std::string token = userTokenGenerator();

  response->set_host(target._host);
  response->set_port(target._port);
  response->set_token(token);
  response->set_error(static_cast<std::size_t>(ServiceStatus::SERVICE_SUCCESS));

  registerUserToken(request->uuid(), token);
  return grpc::Status::OK;
}

::grpc::Status
grpc::GrpcBalancerImpl::UserLoginToServer(::grpc::ServerContext* context,
          const ::message::LoginChattingServer* request,
          ::message::LoginChattingResponse* response)
{
          /*verify user token*/
          response->set_error(
                    static_cast<std::size_t>(verifyUserToken(request->uuid(), request->token()))
          );
          return grpc::Status::OK;
}

std::string grpc::GrpcBalancerImpl::userTokenGenerator() {
  boost::uuids::uuid uuid_gen = boost::uuids::random_generator()();
  return boost::uuids::to_string(uuid_gen);
}
