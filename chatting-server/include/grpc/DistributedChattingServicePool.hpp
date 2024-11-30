#pragma once
#ifndef _DISTIBUTEDCHATTINGSERVICEPOOL_HPP_
#define _DISTIBUTEDCHATTINGSERVICEPOOL_HPP_
#include <config/ServerConfig.hpp>
#include <grpcpp/grpcpp.h>
#include <message/message.grpc.pb.h>
#include <service/ConnectionPool.hpp>
#include <spdlog/spdlog.h>

namespace stubpool {
/*declaration*/
class DistributedChattingServicePool;

/*
 * class DistributedChattingServicePoolImpl
 * using a wrapper to wrap a singleton class
 */
namespace details {
class DistributedChattingServicePoolImpl
    : public connection::ConnectionPool<
          DistributedChattingServicePoolImpl,
          typename message::DistributedChattingService::Stub> {
  friend class DistributedChattingServicePool;
  friend class Singleton<DistributedChattingServicePoolImpl>;

protected:
  DistributedChattingServicePoolImpl()
      : connection::ConnectionPool<self, data_type>() { /*empty*/ }

public:
  using self = DistributedChattingServicePoolImpl;
  using data_type = typename message::DistributedChattingService::Stub;
  using context = data_type;
  using context_ptr = std::unique_ptr<data_type>;

  ~DistributedChattingServicePoolImpl() = default;
};
} // namespace details

/*
 * class DistributedChattingServicePool
 */
class DistributedChattingServicePool
    : public details::DistributedChattingServicePoolImpl {
public:
  DistributedChattingServicePool(const grpc::string &host,
                                 const grpc::string &port)
      : m_host(host), m_port(port), m_cred(grpc::InsecureChannelCredentials()),
        details::DistributedChattingServicePoolImpl() {
    auto address = fmt::format("{}:{}", m_host, m_port);
    spdlog::info("Loading Peer Chatting Servers {}", address);

    /*creating multiple stub*/
    for (std::size_t i = 0; i < m_queue_size; ++i) {
      m_stub_queue.push(std::move(message::DistributedChattingService::NewStub(
          grpc::CreateChannel(address, m_cred))));
    }
  }

  virtual ~DistributedChattingServicePool() = default;

public:
          auto acquire_stub() {
                    return this->acquire();
          }

          void release_stub(std::unique_ptr<message::DistributedChattingService::Stub> stub) {
                    return this->release(std::move(stub));
          }

private:
  grpc::string m_host;
  grpc::string m_port;
  std::shared_ptr<grpc::ChannelCredentials> m_cred;
};
} // namespace stubpool

#endif
