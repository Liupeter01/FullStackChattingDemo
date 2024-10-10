#include <grpc/GrpcBalanceService.hpp>
#include <grpc/GrpcDistributedChattingService.hpp>

gRPCDistributedChattingService::gRPCDistributedChattingService()
{
          /*pass current server name as a parameter to the balance server, and returns all peers*/
          auto response = gRPCBalancerService::getPeerServerLists(ServerConfig::get_instance()->ChattingServerName);

          if (response.error() !=
                    static_cast<int32_t>(ServiceStatus::SERVICE_SUCCESS)) {
                    spdlog::error("[Balance Server] try retrieve peer servers' info failed!, error code {}", response.error());
                    std::abort();
          }

          /*get server lists*/
          auto& peer_servers = response.lists();

          /*traversal server lists and create multiple DistributedChattingServicePool according to host and port*/
          std::for_each(peer_servers.begin(), peer_servers.end(), [this](const message::ChattingPeerServer& server) {
                    m_pools.insert(std::pair<std::string, std::unique_ptr< stubpool::DistributedChattingServicePool>>(
                              /*peer server's name*/ server.name(),
                              /*create a pool*/std::make_unique<stubpool::DistributedChattingServicePool>(server.host(), server.port())
                    ));
          });
}