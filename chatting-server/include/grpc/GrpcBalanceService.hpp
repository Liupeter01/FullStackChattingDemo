#pragma once
#ifndef GRPCBALANCESERVICE_HPP_
#define GRPCBALANCESERVICE_HPP_
#include <grpc/BalanceServicePool.hpp>
#include <grpcpp/client_context.h>
#include <grpcpp/support/status.h>
#include <message/message.grpc.pb.h>
#include <message/message.pb.h>
#include <network/def.hpp>
#include <service/ConnectionPool.hpp>
#include <string_view>

struct gRPCBalancerService {

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

  static message::PeerResponse
  getPeerChattingServerLists(const std::string &cur_name) {
    grpc::ClientContext context;
    message::PeerListsRequest request;
    message::PeerResponse response;

    request.set_cur_server(cur_name);

    connection::ConnectionRAII<stubpool::BalancerServicePool,
                               message::BalancerService::Stub>
        raii;

    grpc::Status status =
        raii->get()->GetPeerChattingServerInfo(&context, request, &response);

    if (!status.ok()) {
      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
    }
    return response;
  }

  static message::PeerResponse
  getPeerGrpcServerLists(const std::string &cur_name) {
    grpc::ClientContext context;
    message::PeerListsRequest request;
    message::PeerResponse response;

    request.set_cur_server(cur_name);

    connection::ConnectionRAII<stubpool::BalancerServicePool,
                               message::BalancerService::Stub>
        raii;

    grpc::Status status =
        raii->get()->GetPeerGrpcServerInfo(&context, request, &response);

    if (!status.ok()) {
      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
    }
    return response;
  }

  static message::GrpcChattingServerResponse registerChattingServerInstance(const std::string& name, const std::string& host,
            const std::string& port) {
            grpc::ClientContext context;
            message::GrpcChattingServerRegRequest request;
            message::GrpcChattingServerResponse response;

            message::ServerInfo info;
            info.set_name(name);
            info.set_host(host);
            info.set_port(port);

            *request.mutable_info() = info;

            connection::ConnectionRAII<stubpool::BalancerServicePool,
                      message::BalancerService::Stub>
                      raii;

            grpc::Status status =
                      raii->get()->RegisterChattingServerInstance(&context, request, &response);

            if (!status.ok()) {
                      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
            }

            return response;
  }

  static message::GrpcChattingServerResponse
  registerGrpcServer(const std::string &name, const std::string &host,
                     const std::string &port) {
    grpc::ClientContext context;
    message::GrpcChattingServerRegRequest request;
    message::GrpcChattingServerResponse response;

    message::ServerInfo info;
    info.set_name(name);
    info.set_host(host);
    info.set_port(port);

    *request.mutable_info() = info;

    connection::ConnectionRAII<stubpool::BalancerServicePool,
                               message::BalancerService::Stub>
        raii;

    grpc::Status status =
        raii->get()->RegisterChattingGrpcServer(&context, request, &response);

    if (!status.ok()) {
      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
    }

    return response;
  }

  static message::GrpcChattingServerResponse
  chattingServerShutdown(const std::string &name) {
    grpc::ClientContext context;
    message::GrpcChattingServerShutdownRequest request;
    message::GrpcChattingServerResponse response;

    request.set_cur_server(name);

    connection::ConnectionRAII<stubpool::BalancerServicePool,
                               message::BalancerService::Stub>
        raii;

    grpc::Status status =
        raii->get()->ChattingServerShutDown(&context, request, &response);

    if (!status.ok()) {
      response.set_error(static_cast<int32_t>(ServiceStatus::GRPC_ERROR));
    }
    return response;
  }
};

#endif // GRPCBALANCESERVICE_HPP_
