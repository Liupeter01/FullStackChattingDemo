#include <config/ServerConfig.hpp>
#include <iostream>
#include <server/GateServer.hpp>
#include <service/IOServicePool.hpp>

int main() {
  try {
    auto &service_pool = IOServicePool::get_instance();
    /*setting up signal*/
    boost::asio::io_context ioc;
    boost::asio::signal_set signal{ioc, SIGINT, SIGTERM};
    signal.async_wait([&ioc, &service_pool](boost::system::error_code ec, int sig_number) {
      if (ec) {
        return;
      }
      service_pool->shutdown();
      ioc.stop();
    });

    std::shared_ptr<GateServer> server = std::make_shared<GateServer>(
              IOServicePool::get_instance()->getIOServiceContext(), // get ioc
              ServerConfig::get_instance()->GateServerPort // get port configuration
    );
    server->serverStart();
    ioc.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
