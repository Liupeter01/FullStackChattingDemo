#include<network/def.hpp>     //network error code
#include<config/ServerConfig.hpp>
#include<grpc/GrpcVerificationService.hpp>

message::GetVerificationResponse  gRPCVerificationService::getVerificationCode(std::string email) {
          grpc::ClientContext context;
          message::GetVerificationRequest request;
          message::GetVerificationResponse response;
          request.set_email(email);

          std::shared_ptr<stubpool::StubRAII> stub(std::make_shared<stubpool::StubRAII>());
          grpc::Status status = stub->GetVerificationCode(&context, request, &response);

          /*error occured*/
          if (!status.ok()) {
                    response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
          }
          return response;
}
