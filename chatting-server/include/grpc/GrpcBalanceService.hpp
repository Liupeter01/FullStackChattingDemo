#pragma once
#ifndef GRPCBALANCESERVICE_HPP_
#define GRPCBALANCESERVICE_HPP_
#include <message/message.grpc.pb.h>
#include <message/message.pb.h>
#include <service/ConnectionPool.hpp>
#include <grpcpp/client_context.h>
#include <grpcpp/support/status.h>
#include <grpc/BalanceServicePool.hpp>
#include <network/def.hpp>

struct gRPCBalancerService {
  // pass user's uuid parameter to the server, and returns available server
  // address to user
  static message::GetAllocatedChattingServer
  addNewUserToServer(std::size_t uuid) {
    grpc::ClientContext context;
    message::RegisterToBalancer request;
    message::GetAllocatedChattingServer response;
    request.set_uuid(uuid);

    connection::ConnectionRAII<stubpool::BalancerServicePool,
                               message::BalancerService::Stub>
        raii;

    grpc::Status status =
        raii->get()->AddNewUserToServer(&context, request, &response);

    ///*error occured*/
    if (!status.ok()) {
      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
    }
    return response;
  }

  static message::LoginChattingResponse
  userLoginToServer(std::size_t uuid, const std::string &token) {
    grpc::ClientContext context;
    message::LoginChattingServer request;
    message::LoginChattingResponse response;
    request.set_uuid(uuid);
    request.set_token(token);

    connection::ConnectionRAII<stubpool::BalancerServicePool,
                               message::BalancerService::Stub>
        raii;

    grpc::Status status =
        raii->get()->UserLoginToServer(&context, request, &response);

    ///*error occured*/
    if (!status.ok()) {
      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
    }
    return response;
  }

  static message::PeerResponse getPeerServerLists(const std::string &cur_name) {
    grpc::ClientContext context;
    message::GetChattingSeverPeerListsRequest request;
    message::PeerResponse response;

    request.set_cur_server_name(cur_name);

    connection::ConnectionRAII<stubpool::BalancerServicePool,
                               message::BalancerService::Stub>
        raii;

    grpc::Status status =
        raii->get()->GetPeerServerInfo(&context, request, &response);

    if (!status.ok()) {
      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
    }
    return response;
  }
};

#endif // BALANCE
