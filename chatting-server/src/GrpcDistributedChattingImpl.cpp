#include <grpc/GrpcDistributedChattingImpl.hpp>

grpc::GrpcDistributedChattingImpl::GrpcDistributedChattingImpl() {}

grpc::GrpcDistributedChattingImpl::~GrpcDistributedChattingImpl() {}

// A send friend request message to another user B
::grpc::Status grpc::GrpcDistributedChattingImpl::SendFriendRequest(
    ::grpc::ServerContext *context,
    const ::message::AddNewFriendRequest *request,
    ::message::AddNewFriendResponse *response) {

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
