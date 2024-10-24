#ifndef USERFRIENDREQUEST_HPP
#define USERFRIENDREQUEST_HPP

#include <memory>
#include <QString>

struct UserFriendRequest{
    UserFriendRequest(const QString &from, const QString &to, const QString &nick, const QString &msg)
        :src_uuid(from), dst_uuid(to), nickname(nick), message(msg)
    {}
    QString src_uuid;
    QString dst_uuid;
    QString nickname;
    QString message;
};

#endif // USERFRIENDREQUEST_HPP
