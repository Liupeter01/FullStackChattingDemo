#pragma once
#ifndef _BALANCESERVICEPOOL_HPP_
#define _BALANCESERVICEPOOL_HPP_
#include <config/ServerConfig.hpp>
#include <grpcpp/grpcpp.h>
#include <message/message.grpc.pb.h>
#include <service/ConnectionPool.hpp>
#include <spdlog/spdlog.h>

namespace stubpool {
class BalancerServicePool
    : public connection::ConnectionPool<
          BalancerServicePool, typename message::BalancerService::Stub> {
  using self = BalancerServicePool;
  using data_type = typename message::BalancerService::Stub;
  using context = data_type;
  using context_ptr = std::unique_ptr<data_type>;
  friend class Singleton<BalancerServicePool>;

  grpc::string m_host;
  grpc::string m_port;
  std::shared_ptr<grpc::ChannelCredentials> m_cred;

  BalancerServicePool()
      : connection::ConnectionPool<self, data_type>(),
        m_host(ServerConfig::get_instance()->BalanceServiceAddress),
        m_port(ServerConfig::get_instance()->BalanceServicePort),
        m_cred(grpc::InsecureChannelCredentials()) {

    auto address = fmt::format("{}:{}", m_host, m_port);
    spdlog::info("Connected to balance server {}", address);

    /*creating multiple stub*/
    for (std::size_t i = 0; i < m_queue_size; ++i) {
      m_stub_queue.push(std::move(message::BalancerService::NewStub(
          grpc::CreateChannel(address, m_cred))));
    }
  }

public:
  ~BalancerServicePool() = default;
};
} // namespace stubpool

#endif // !_GRPCBALANCESERVICE_HPP_
