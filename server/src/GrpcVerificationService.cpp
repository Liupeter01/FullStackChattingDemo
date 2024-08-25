#include <config/ServerConfig.hpp>
#include <grpc/GrpcVerificationService.hpp>
#include <network/def.hpp> //network error code

message::GetVerificationResponse
gRPCVerificationService::getVerificationCode(std::string email) {
  grpc::ClientContext context;
  message::GetVerificationRequest request;
  message::GetVerificationResponse response;
  request.set_email(email);

  connection::ConnectionRAII<stubpool::VerificationServicePool,
                             message::VerificationService::Stub>
      raii;

  grpc::Status status =
      raii->get()->GetVerificationCode(&context, request, &response);

  /*error occured*/
  if (!status.ok()) {
    response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
  }
  return response;
}
