#include <json/value.h>
#include <spdlog/spdlog.h>
#include <server/Session.hpp>
#include <server/UserManager.hpp>
#include <config/ServerConfig.hpp>
#include <grpc/GrpcDistributedChattingImpl.hpp>

grpc::GrpcDistributedChattingImpl::GrpcDistributedChattingImpl() {}

grpc::GrpcDistributedChattingImpl::~GrpcDistributedChattingImpl() {}

// A send friend request message to another user B
::grpc::Status grpc::GrpcDistributedChattingImpl::SendFriendRequest(
    ::grpc::ServerContext *context,
    const ::message::AddNewFriendRequest *request,
    ::message::AddNewFriendResponse *response) {
          auto src = request->src_uuid();
          auto dst = request->dst_uuid();
          auto nickname = request->nick_name();
          auto msg = request->req_msg();

          /*
           * try to locate target user id in this server's user management mapping ds
           * Maybe we can not find this user in the server
           */
          std::optional<std::shared_ptr<Session>> session_op = UserManager::get_instance()->getSession(std::to_string(request->dst_uuid()));
          if (!session_op.has_value()) {   
                    spdlog::warn("[GRPC {} Service]: Find Target User {} Error!", ServerConfig::get_instance()->GrpcServerName, dst);
                    response->set_error(static_cast<uint8_t>(ServiceStatus::FRIENDING_TARGET_USER_NOT_FOUND));
          }
          else {
                    /*setup json data and forward to target user*/
                    Json::Value root;
                    root["error"] = static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS);
                    root["src_uuid"] = src;
                    root["dst_uuid"] = dst;
                    root["nickname"] = nickname;
                    root["message"] = msg;

                    session_op.value()->sendMessage(ServiceType::SERVICE_FRIENDREINCOMINGREQUEST, root.toStyledString());

                    /*setup response*/
                    response->set_src_uuid(src);
                    response->set_dst_uuid(dst);
                    response->set_error(static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS));
          }

          return grpc::Status::OK;
}

// User B agreed with user A's friend adding request
::grpc::Status grpc::GrpcDistributedChattingImpl::ConfirmFriendRequest(
    ::grpc::ServerContext *context, const ::message::AuthoriseRequest *request,
    ::message::AuthoriseResponse *response) {

  return grpc::Status::OK;
}

// Verify that B is still A's friend:
::grpc::Status grpc::GrpcDistributedChattingImpl::FriendshipVerification(
    ::grpc::ServerContext *context, const ::message::AuthoriseRequest *request,
    ::message::AuthoriseResponse *response) {

  return grpc::Status::OK;
}

// transfer chatting message from user A to B
::grpc::Status grpc::GrpcDistributedChattingImpl::NormalChattingMsg(
    ::grpc::ServerContext *context,
    const ::message::SendChattingMsgRequest *request,
    ::message::SendChattingMsgResponse *response) {

  return grpc::Status::OK;
}
