#pragma once
#ifndef _VERIFICATIONSERVICEPOOL_HPP_
#define _VERIFICATIONSERVICEPOOL_HPP_

#include<spdlog/spdlog.h>
#include <grpcpp/grpcpp.h>
#include<config/ServerConfig.hpp>
#include<service/ConnectionPool.hpp>
#include <message/message.grpc.pb.h>

namespace stubpool {
          class VerificationServicePool
                    :public connection::ConnectionPool<VerificationServicePool, typename message::VerificationService::Stub>
          {
                    using self = VerificationServicePool;
                    using data_type = typename message::VerificationService::Stub;
                    friend class Singleton<VerificationServicePool>;

                    grpc::string m_addr;
                    std::shared_ptr<grpc::ChannelCredentials> m_cred;

                    VerificationServicePool()
                              : connection::ConnectionPool<self, data_type>()
                              , m_addr(ServerConfig::get_instance()->VerificationServerAddress)
                              , m_cred(grpc::InsecureChannelCredentials()){
                              spdlog::info("Connected to verification server addr {}", m_addr.c_str());

                              /*creating multiple stub*/
                              for (std::size_t i = 0; i < m_queue_size; ++i) {
                                        m_stub_queue.push(std::move(message::VerificationService::NewStub(
                                                  grpc::CreateChannel(m_addr, m_cred))));
                              }
                    }

          public:
                    ~VerificationServicePool() = default;
          };
}

#endif // !_STUBPOOL_HPP_
