#pragma once
#ifndef _GRPCDISTRIBUTEDCHATTINGSERVICE_HPP_
#define _GRPCDISTRIBUTEDCHATTINGSERVICE_HPP_
#include <grpc/BalanceServicePool.hpp>
#include <grpc/DistributedChattingServicePool.hpp>
#include <network/def.hpp>
#include <unordered_map>

class gRPCDistributedChattingService
    : public Singleton<gRPCDistributedChattingService> {
  friend class Singleton<gRPCDistributedChattingService>;
  gRPCDistributedChattingService();

public:
  virtual ~gRPCDistributedChattingService() = default;

private:
  std::unordered_map<
      /*server_name*/ std::string,
      /*DistributedChattingServicePool*/ std::unique_ptr<
          stubpool::DistributedChattingServicePool>>
      m_pools;
};

#endif //_GRPCDISTRIBUTEDCHATTINGSERVICE_HPP_
