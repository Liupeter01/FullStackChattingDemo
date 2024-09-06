#include <config/ServerConfig.hpp>
#include <server/AsyncServer.hpp>
#include <service/IOServicePool.hpp>
#include <spdlog/spdlog.h>
#include <sql/MySQLConnectionPool.hpp>

int main() {
  try {
    [[maybe_unused]] auto &service_pool = IOServicePool::get_instance();
    [[maybe_unused]] auto &mysql = mysql::MySQLConnectionPool::get_instance();

    /*setting up signal*/
    boost::asio::io_context ioc;
    boost::asio::signal_set signal{ioc, SIGINT, SIGTERM};
    signal.async_wait(
        [&ioc, &service_pool](boost::system::error_code ec, int sig_number) {
          if (ec) {
            return;
          }
          ioc.stop();
          service_pool->shutdown();
        });

    /*create chatting server*/
    std::shared_ptr<AsyncServer> server = std::make_shared<AsyncServer>(
        service_pool->getIOServiceContext(),
        ServerConfig::get_instance()->ChattingServerPort);

    server->startAccept();
    /**/
    ioc.run();
  } catch (const std::exception &e) {
    spdlog::error("{}", e.what());
  }
  return 0;
}
