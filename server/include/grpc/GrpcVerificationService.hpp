#pragma once
#ifndef GRPCVERIFICATIONSERVICE_HPP_
#define GRPCVERIFICATIONSERVICE_HPP_
#include <grpc/VerificationServicePool.hpp>

struct gRPCVerificationService {
  static message::GetVerificationResponse
  getVerificationCode(std::string email);
};

#endif
