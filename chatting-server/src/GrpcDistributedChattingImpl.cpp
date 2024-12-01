#include <config/ServerConfig.hpp>
#include <grpc/GrpcDistributedChattingImpl.hpp>
#include <handler/SyncLogic.hpp>
#include <json/value.h>
#include <server/Session.hpp>
#include <server/UserManager.hpp>
#include <server/UserNameCard.hpp>
#include <spdlog/spdlog.h>

grpc::GrpcDistributedChattingImpl::GrpcDistributedChattingImpl() {}

grpc::GrpcDistributedChattingImpl::~GrpcDistributedChattingImpl() {}

// A send friend request message to another user B
::grpc::Status grpc::GrpcDistributedChattingImpl::SendFriendRequest(
    ::grpc::ServerContext *context, const ::message::FriendRequest *request,
    ::message::FriendResponse *response) {
  /*
   * try to locate target user id in this server's user management mapping ds
   * Maybe we can not find this user in the server
   */
  std::optional<std::shared_ptr<Session>> session_op =
      UserManager::get_instance()->getSession(
          std::to_string(request->dst_uuid()));
  if (!session_op.has_value()) {
    spdlog::warn("[GRPC {} Service]: Find Target User {} Error!",
                 ServerConfig::get_instance()->GrpcServerName,
                 request->dst_uuid());
    response->set_error(
        static_cast<uint8_t>(ServiceStatus::FRIENDING_TARGET_USER_NOT_FOUND));
  } else {
    /*setup json data and forward to target user*/
    Json::Value root;
    root["error"] = static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS);
    root["src_uuid"] = std::to_string(request->src_uuid());
    root["dst_uuid"] = std::to_string(request->dst_uuid());
    root["src_nickname"] = request->nick_name();
    root["src_message"] = request->req_msg();
    root["src_avator"] = request->avator_path();
    root["src_username"] = request->username();
    root["src_desc"] = request->description();
    root["src_sex"] = request->sex();

    /*send a forwarding packet*/
    session_op.value()->sendMessage(
        ServiceType::SERVICE_FRIENDREINCOMINGREQUEST, root.toStyledString());

    /*setup response*/
    response->set_src_uuid(request->src_uuid());
    response->set_dst_uuid(request->dst_uuid());
    response->set_error(static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS));
  }

  return grpc::Status::OK;
}

// User B agreed with user A's friend adding request
::grpc::Status grpc::GrpcDistributedChattingImpl::ConfirmFriendRequest(
    ::grpc::ServerContext *context, const ::message::FriendRequest *request,
    ::message::FriendResponse *response) {

  /*
   * try to locate target user id in this server's user management mapping ds
   * Maybe we can not find this user in the server
   */
  std::optional<std::shared_ptr<Session>> session_op =
      UserManager::get_instance()->getSession(
          std::to_string(request->src_uuid()));
  if (!session_op.has_value()) {
    spdlog::warn("[GRPC {} Service]: Find Target User {} Error!",
                 ServerConfig::get_instance()->GrpcServerName,
                 request->src_uuid());
    response->set_error(
        static_cast<uint8_t>(ServiceStatus::FRIENDING_TARGET_USER_NOT_FOUND));
  } else {
    /*setup json data and forward to target user*/
    Json::Value root;

    root["error"] = static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS);
    root["friend_uuid"] = request->dst_uuid();
    root["friend_nickname"] = request->nick_name();
    root["friend_avator"] = request->avator_path();
    root["friend_username"] = request->username();
    root["friend_desc"] = request->description();
    root["friend_sex"] = request->sex();

    /*send a forwarding packet*/
    session_op.value()->sendMessage(
        ServiceType::SERVICE_FRIENDING_ON_BIDDIRECTIONAL,
        root.toStyledString());

    /*setup response*/
    response->set_src_uuid(request->src_uuid());
    response->set_dst_uuid(request->dst_uuid());
    response->set_error(static_cast<uint8_t>(ServiceStatus::SERVICE_SUCCESS));
  }

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
