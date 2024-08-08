#pragma once
#ifndef GRPCVERIFICATIONSERVICE_HPP_
#define GRPCVERIFICATIONSERVICE_HPP_
#include<grpc/StubPool.hpp>

class gRPCVerificationService :public Singleton< gRPCVerificationService> 
{
          friend class Singleton< gRPCVerificationService>;
          gRPCVerificationService() = default;

public:
          ~gRPCVerificationService() = default;
          static message::GetVerificationResponse getVerificationCode(std::string email);
};

#endif