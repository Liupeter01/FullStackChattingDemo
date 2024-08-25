#pragma once
#ifndef _VERIFICATIONSERVICEPOOL_HPP_
#define _VERIFICATIONSERVICEPOOL_HPP_
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

#include <grpcpp/grpcpp.h>
#include <message/message.grpc.pb.h>
#include <singleton/singleton.hpp>
#include <tools/tools.hpp>

namespace stubpool {
namespace details {
class VerificationServicePool : public Singleton<VerificationServicePool > {
  friend class Singleton<VerificationServicePool >;
  VerificationServicePool();

public:
  using stub = message::VerificationService::Stub;
  using stub_ptr = std::unique_ptr<stub>;

  ~VerificationServicePool();
  void shutdown();

  std::optional<stub_ptr> acquire();
  void release(stub_ptr stub);

private:
  /*Stubpool stop flag*/
  std::atomic<bool> m_stop;

  /*Stub Ammount*/
  std::size_t m_queue_size;

  /*record address info and credentials*/
  grpc::string m_addr;
  std::shared_ptr<grpc::ChannelCredentials> m_cred;

  /*queue control*/
  std::mutex m_mtx;
  std::condition_variable m_cv;

  /*stub queue*/
  std::queue<stub_ptr> m_stub_queue;
};
} // namespace details

/*
 * get stub automatically!
 */
class VerificationServiceRAII {
  using wrapper = tools::ResourcesWrapper<details::VerificationServicePool::stub>;

public:
          VerificationServiceRAII();
  ~VerificationServiceRAII();
  std::optional<wrapper> operator->();

private:
  bool status; // load stub success flag
  details::VerificationServicePool::stub_ptr m_stub;
};
} // namespace stubpool

#endif // !_STUBPOOL_HPP_
