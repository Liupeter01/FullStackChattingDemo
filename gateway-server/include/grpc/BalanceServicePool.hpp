#pragma once
#ifndef _CHATTINGERVICEPOOL_HPP_
#define _CHATTINGSERVICEPOOL_HPP_
#include <config/ServerConfig.hpp>
#include <grpcpp/grpcpp.h>
#include <message/message.grpc.pb.h>
#include <service/ConnectionPool.hpp>
#include <spdlog/spdlog.h>

namespace stubpool {
class ChattingServicePool
    : public connection::ConnectionPool<
          ChattingServicePool,
          typename message::ChattingServiceBalancer::Stub> {
  using self = ChattingServicePool;
  using data_type = typename message::ChattingServiceBalancer::Stub;
  using context = data_type;
  using context_ptr = std::unique_ptr<data_type>;
  friend class Singleton<ChattingServicePool>;

  grpc::string m_host;
  grpc::string m_port;
  std::shared_ptr<grpc::ChannelCredentials> m_cred;

  ChattingServicePool()
      : connection::ConnectionPool<self, data_type>(),
        m_host(ServerConfig::get_instance()->BalanceServiceAddress),
        m_port(ServerConfig::get_instance()->BalanceServicePort),
        m_cred(grpc::InsecureChannelCredentials()) {

    auto address = fmt::format("{}:{}", m_host, m_port);
    spdlog::info("Connected to balance server {}", address);

    /*creating multiple stub*/
    for (std::size_t i = 0; i < m_queue_size; ++i) {
      m_stub_queue.push(std::move(message::ChattingServiceBalancer::NewStub(
          grpc::CreateChannel(address, m_cred))));
    }
  }

public:
  ~ChattingServicePool() = default;
};
} // namespace stubpool

#endif // !_STUBPOOL_HPP_
