#pragma once
#ifndef GRPCVERIFICATIONSERVICE_HPP_
#define GRPCVERIFICATIONSERVICE_HPP_

#include <network/def.hpp>
#include <grpc/VerificationServicePool.hpp>

struct gRPCVerificationService {
            static message::GetVerificationResponse getVerificationCode(std::string email) {
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
};

#endif
