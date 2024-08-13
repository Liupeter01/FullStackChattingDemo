#include <config/ServerConfig.hpp>
#include <grpc/StubPool.hpp>
#include <thread>

stubpool::details::StubPool::StubPool()
    : m_stop(false), m_queue_size(std::thread::hardware_concurrency()),
      m_addr(ServerConfig::get_instance()->VerificationServerAddress),
      m_cred(grpc::InsecureChannelCredentials()) {
  printf("connected to verification server addr = %s\n", m_addr.c_str());

  /*creating multiple stub*/
  for (std::size_t i = 0; i < m_queue_size; ++i) {
    m_stub_queue.push(std::move(message::VerificationService::NewStub(
        grpc::CreateChannel(m_addr, m_cred))));
  }
}

stubpool::details::StubPool::~StubPool() { shutdown(); }

void stubpool::details::StubPool::shutdown() {
  /*set stop flag to true*/
  m_stop = true;
  m_cv.notify_all();

  std::lock_guard<std::mutex> _lckg(m_mtx);
  while (!m_stub_queue.empty()) {
    m_stub_queue.pop();
  }
}

std::optional<stubpool::details::StubPool::stub_ptr>
stubpool::details::StubPool::acquire() {
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

void stubpool::details::StubPool::release(StubPool::stub_ptr stub) {
  if (m_stop) {
    return;
  }
  std::lock_guard<std::mutex> _lckg(m_mtx);
  m_stub_queue.push(std::move(stub));
  m_cv.notify_one();
}

stubpool::StubRAII::StubRAII() : status(true) {
  auto optional = stubpool::details::StubPool::get_instance()->acquire();
  if (!optional.has_value()) {
    status = false;
  } else {
    m_stub = std::move(optional.value());
  }
}

stubpool::StubRAII::~StubRAII() {
  if (status) {
    stubpool::details::StubPool::get_instance()->release(std::move(m_stub));

    /*StubRAII failed!!*/
    status = false;
  }
}

std::optional<stubpool::StubRAII::wrapper> stubpool::StubRAII::operator->() {
  if (status) {
    return wrapper(m_stub.get());
  }
  return std::nullopt;
}
