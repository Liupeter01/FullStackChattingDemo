#ifndef USERFRIENDREQUEST_HPP
#define USERFRIENDREQUEST_HPP
#include <server/UserNameCard.hpp>

struct UserFriendRequest : public UserNameCard{
    UserFriendRequest(const std::string&from,
                      const std::string&to,
                      const std::string&nick,
                      const std::string&msg,
                      const std::string&avator_path,
                      const std::string&username,
                      const std::string&desc,
                      Sex sex)
        : UserNameCard(from, avator_path, username, nick, desc, sex)
        , dst_uuid(to)
        , message(msg)
    {}
    std::string dst_uuid;
    std::string message;
};

#endif // USERFRIENDREQUEST_HPP
