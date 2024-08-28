#pragma once
#ifndef _GRPCBALANCERIMPL_HPP_
#define _GRPCBALANCERIMPL_HPP_
#include <grpcpp/grpcpp.h>
#include <message/message.grpc.pb.h>
#include <mutex>
#include <optional>
#include <string_view>
#include <unordered_map>

namespace grpc {
struct ChattingServerConfig {
  std::string _host;
  std::string _port;
  std::string _name;
  std::size_t _connections;
};

class GrpcBalancerImpl final
    : public message::ChattingServiceBalancer::Service {
public:
  ~GrpcBalancerImpl();
  GrpcBalancerImpl();

public:
  ::grpc::Status
  AddNewUserToServer(::grpc::ServerContext *context,
                     const ::message::RegisterToBalancer *request,
                     ::message::GetAllocatedChattingServer *response);

  static std::string userTokenGenerator();

private:
  const grpc::ChattingServerConfig &serverLoadBalancer();
  void registerUserToken(std::size_t uuid, const std::string &tokens);
  std::optional<std::string_view> getUserToken(std::size_t uuid);

private:
  std::mutex server_mtx;
  std::mutex token_mtx;
  std::unordered_map<
      /*server name*/ std::string,
      /*server info*/ ChattingServerConfig>
      servers;

  std::unordered_map<
      /*uuid*/ std::size_t,
      /*tokens*/ std::string>
      tokens;
};
} // namespace grpc
#endif
