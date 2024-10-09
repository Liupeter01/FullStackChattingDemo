#pragma once
#ifndef _GRPCBALANCERIMPL_HPP_
#define _GRPCBALANCERIMPL_HPP_
#include <grpcpp/grpcpp.h>
#include <message/message.grpc.pb.h>
#include <mutex>
#include <network/def.hpp>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace grpc {
class GrpcBalancerImpl final : public message::BalancerService::Service {

public:
  struct ChattingServerConfig {
    std::string _host;
    std::string _port;
    std::string _name;
    std::size_t _connections = 0; /*add init*/
  };

  ~GrpcBalancerImpl();
  GrpcBalancerImpl();

public:
  ::grpc::Status
  AddNewUserToServer(::grpc::ServerContext *context,
                     const ::message::RegisterToBalancer *request,
                     ::message::GetAllocatedChattingServer *response);

  ::grpc::Status
  UserLoginToServer(::grpc::ServerContext *context,
                    const ::message::LoginChattingServer *request,
                    ::message::LoginChattingResponse *response);

  ::grpc::Status
  GetPeerServerInfo(::grpc::ClientContext *context,
                    const ::message::GetChattingSeverPeerListsRequest *request,
                    ::message::PeerResponse *response);

  static std::string userTokenGenerator();

private:
  const grpc::GrpcBalancerImpl::ChattingServerConfig &serverLoadBalancer();
  void
  registerUserInfo(std::size_t uuid, std::string &&tokens,
                   const grpc::GrpcBalancerImpl::ChattingServerConfig &server);
  std::optional<std::string_view> getUserToken(std::size_t uuid);
  ServiceStatus verifyUserToken(std::size_t uuid, const std::string &tokens);

private:
  struct UserInfo {
    UserInfo(std::string &&tokens,
             const grpc::GrpcBalancerImpl::ChattingServerConfig &config);

    /*user token*/
    std::string m_tokens;

    /*server info*/
    std::string_view m_host;
    std::string_view m_port;
  };

  std::mutex server_mtx;
  std::mutex token_mtx;
  std::unordered_map<
      /*server name*/ std::string,
      /*server info*/ ChattingServerConfig>
      servers;

  std::unordered_map<
      /*uuid*/ std::size_t,
      /*user token and belonged server*/ std::shared_ptr<UserInfo>>
      users;
};
} // namespace grpc
#endif
