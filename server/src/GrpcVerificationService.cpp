#include<network/def.hpp>     //network error code
#include<config/ServerConfig.hpp>
#include<grpc/GrpcVerificationService.hpp>

gRPCVerificationService::gRPCVerificationService()
          :gRPCVerificationService(ServerConfig::get_instance()->VerificationServerAddress)
{
          printf("connected to verification server addr = %s",
                    ServerConfig::get_instance()->VerificationServerAddress.c_str());
}

gRPCVerificationService::gRPCVerificationService(grpc::string ip_port, std::shared_ptr<grpc::ChannelCredentials> cred) 
          : m_stub(std::move(message::VerificationService::NewStub(grpc::CreateChannel(ip_port, cred))))
{
}

message::GetVerificationResponse  gRPCVerificationService::getVerificationCode(std::string email) {
          grpc::ClientContext context;
          message::GetVerificationRequest request;
          message::GetVerificationResponse response;
          request.set_email(email);

          grpc::Status status = m_stub->GetVerificationCode(&context, request, &response);

          /*error occured*/
          if (!status.ok()) {
                    response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
          }
          return response;
}
