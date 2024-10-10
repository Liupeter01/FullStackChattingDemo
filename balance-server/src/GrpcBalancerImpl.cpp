#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <config/ServerConfig.hpp>
#include <grpc/GrpcBalancerImpl.hpp>
#include <network/def.hpp>

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
  decltype(servers)::const_iterator min = servers.begin();
  for (auto ib = servers.begin(); ib != servers.end(); ib++) {
    if (ib->second._connections < min->second._connections) {
      min = ib;
    }
  }
  return min->second;
}

std::optional<std::string_view>
grpc::GrpcBalancerImpl::getUserToken(std::size_t uuid) {
  auto target = users.find(uuid);
  if (target == users.end()) {
    return std::nullopt;
  }
  return target->second->m_tokens;
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
  std::lock_guard<std::mutex> _lckg(token_mtx);
  users[uuid] = std::make_shared<UserInfo>(std::move(tokens), server);
}

::grpc::Status grpc::GrpcBalancerImpl::AddNewUserToServer(
    ::grpc::ServerContext *context,
    const ::message::RegisterToBalancer *request,
    ::message::GetAllocatedChattingServer *response) {

  auto uuid = request->uuid();
  std::optional<std::string_view> exists = getUserToken(uuid);

  /*check if it is registered?*/
  if (!exists.has_value()) {
    /*get the lowest load server*/
    auto target = serverLoadBalancer();

    /*generate a user token and store it into the structure first*/
    std::string token = userTokenGenerator();

    response->set_host(target._host);
    response->set_port(target._port);
    response->set_token(token);
    response->set_error(
        static_cast<std::size_t>(ServiceStatus::SERVICE_SUCCESS));

    registerUserInfo(uuid, std::move(token), target);
  } else {
    response->set_error(
        static_cast<std::size_t>(ServiceStatus::LOGIN_FOR_MULTIPLE_TIMES));
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
