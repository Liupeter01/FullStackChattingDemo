#include <config/ServerConfig.hpp>
#include <server/AsyncServer.hpp>
#include <service/IOServicePool.hpp>
#include <spdlog/spdlog.h>
#include <handler/SyncLogic.hpp>
#include <redis/RedisManager.hpp>
#include <sql/MySQLConnectionPool.hpp>
#include <grpc/BalanceServicePool.hpp>
#include <grpc/DistributedChattingServicePool.hpp>
#include <grpc/GrpcDistributedChattingImpl.hpp>

//redis_server_login hash
static std::string redis_server_login = "redis_server";  

int main() {
  try {
            [[maybe_unused]] auto& service_pool = IOServicePool::get_instance();
            [[maybe_unused]] auto& mysql = mysql::MySQLConnectionPool::get_instance();
            [[maybe_unused]] auto& redis = redis::RedisConnectionPool::get_instance();
            [[maybe_unused]] auto& balance =
                      stubpool::BalancerServicePool::get_instance();
            [[maybe_unused]] auto& distribute =
                      stubpool::DistributedChattingServicePool::get_instance();

            std::string address =
                      fmt::format("{}:{}", ServerConfig::get_instance()->ChattingServerHost,
                                ServerConfig::get_instance()->ChattingServerPort);

            spdlog::info("Current Chatting RPC Server Started Running On {}", address);

            /*gRPC server*/
            grpc::ServerBuilder builder;
            grpc::GrpcDistributedChattingImpl impl;

            /*binding ports and establish service*/
            builder.AddListeningPort(address, grpc::InsecureServerCredentials());
            builder.RegisterService(&impl);

            std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
            
            /*execute grpc server in another thread*/
            std::thread grpc_server_thread([&server]() {
                      server->Wait();
                      });

    /*setting up signal*/
    boost::asio::io_context ioc;
    boost::asio::signal_set signal{ioc, SIGINT, SIGTERM};
    signal.async_wait(
        [&ioc, &service_pool, &server](boost::system::error_code ec, int sig_number) {
          if (ec) {
            return;
          }
          spdlog::critical("chatting server exit due to control-c input!");
          ioc.stop();
          service_pool->shutdown();
          server->Shutdown();
        });

    /*set current server connection counter value(0) to hash by using HSET*/ 
    connection::ConnectionRAII<redis::RedisConnectionPool, redis::RedisContext> raii;
    raii->get()->setValue2Hash(redis_server_login,
              ServerConfig::get_instance()->ChattingServerName, std::to_string(0));
    

    /*create chatting server*/
    std::shared_ptr<AsyncServer> async = std::make_shared<AsyncServer>(
        service_pool->getIOServiceContext(),
        ServerConfig::get_instance()->ChattingServerPort);

    async->startAccept();
    /**/
    ioc.run();

    /*join subthread*/
    if (grpc_server_thread.joinable()) {
              grpc_server_thread.join();
    }

    /*
    * Chatting server shutdown
    * Delete current chatting server connection counter by using HDEL
    */
    raii->get()->delValueFromHash(redis_server_login,
              ServerConfig::get_instance()->ChattingServerName);

  } catch (const std::exception &e) {
    spdlog::error("{}", e.what());
  }
  return 0;
}
