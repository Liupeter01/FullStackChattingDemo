#ifndef USERFRIENDREQUEST_HPP
#define USERFRIENDREQUEST_HPP

#include <QString>
#include <UserNameCard.h>
#include <memory>

struct UserFriendRequest : public UserNameCard {
  UserFriendRequest(const QString &from, const QString &to, const QString &nick,
                    const QString &msg, const QString &avator_path,
                    const QString &username, const QString &desc, Sex sex)
      : UserNameCard(from, avator_path, username, nick, desc, sex),
        dst_uuid(to), message(msg) {}
  QString dst_uuid;
  QString message;
};

#endif // USERFRIENDREQUEST_HPP
