#include <config/ServerConfig.hpp>
#include <grpc/VerificationServicePool.hpp>
#include <spdlog/spdlog.h>
#include <thread>

stubpool::details::VerificationServicePool::VerificationServicePool()
    : m_stop(false), m_queue_size(std::thread::hardware_concurrency()),
      m_addr(ServerConfig::get_instance()->VerificationServerAddress),
      m_cred(grpc::InsecureChannelCredentials()) {

  spdlog::info("Connected to verification server addr {}", m_addr.c_str());
  /*creating multiple stub*/
  for (std::size_t i = 0; i < m_queue_size; ++i) {
    m_stub_queue.push(std::move(message::VerificationService::NewStub(
        grpc::CreateChannel(m_addr, m_cred))));
  }
}

stubpool::details::VerificationServicePool::~VerificationServicePool() { shutdown(); }

void stubpool::details::VerificationServicePool::shutdown() {
  /*set stop flag to true*/
  m_stop = true;
  m_cv.notify_all();

  std::lock_guard<std::mutex> _lckg(m_mtx);
  while (!m_stub_queue.empty()) {
    m_stub_queue.pop();
  }
}

std::optional<stubpool::details::VerificationServicePool::stub_ptr>
stubpool::details::VerificationServicePool::acquire() {
  std::unique_lock<std::mutex> _lckg(m_mtx);
  m_cv.wait(_lckg, [this]() { return !m_stub_queue.empty() || m_stop; });

  /*check m_stop flag*/
  if (m_stop) {
    return std::nullopt;
  }
  stub_ptr temp = std::move(m_stub_queue.front());
  m_stub_queue.pop();
  return temp;
}

void stubpool::details::VerificationServicePool::release(VerificationServicePool::stub_ptr stub) {
  if (m_stop) {
    return;
  }
  std::lock_guard<std::mutex> _lckg(m_mtx);
  m_stub_queue.push(std::move(stub));
  m_cv.notify_one();
}

stubpool::VerificationServiceRAII::VerificationServiceRAII() : status(true) {
  auto optional = stubpool::details::VerificationServicePool::get_instance()->acquire();
  if (!optional.has_value()) {
    status = false;
  } else {
    m_stub = std::move(optional.value());
  }
}

stubpool::VerificationServiceRAII::~VerificationServiceRAII() {
  if (status) {
    stubpool::details::VerificationServicePool::get_instance()->release(std::move(m_stub));

    /*StubRAII failed!!*/
    status = false;
  }
}

std::optional<stubpool::VerificationServiceRAII::wrapper> stubpool::VerificationServiceRAII ::operator->() {
  if (status) {
    return wrapper(m_stub.get());
  }
  return std::nullopt;
}
