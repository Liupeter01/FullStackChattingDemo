#pragma once
#ifndef GRPCVERIFICATIONSERVICE_HPP_
#define GRPCVERIFICATIONSERVICE_HPP_
#include<grpcpp/grpcpp.h>
#include<message/message.grpc.pb.h>
#include<singleton/singleton.hpp>

class gRPCVerificationService :public Singleton< gRPCVerificationService> 
{
          friend class Singleton< gRPCVerificationService>;
public:
          ~gRPCVerificationService() = default;
          message::GetVerificationResponse getVerificationCode(std::string email);

private:
          gRPCVerificationService();
          gRPCVerificationService(
                    grpc::string ip_port,
                    std::shared_ptr<grpc::ChannelCredentials> cred = grpc::InsecureChannelCredentials()
          );

private:
          std::unique_ptr<message::VerificationService::Stub> m_stub;
};

#endif