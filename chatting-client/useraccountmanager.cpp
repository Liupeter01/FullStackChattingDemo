#include <QJsonArray>
#include<useraccountmanager.hpp>

void UserAccountManager::appendFriendRequestList(const QJsonArray& array){
    for(const  QJsonValue &obj : array){
        addItem2FriendRequestList(
            std::make_shared<UserFriendRequest>(
                obj["src_uuid"].toString(),
                obj["dst_uuid"].toString(),
                obj["nickname"].toString(),
                obj["message"].toString(),
                obj["avator"].toString(),
                obj["username"].toString(),
                obj["description"].toString(),
                static_cast<Sex>(obj["sex"].toInt())
        ));
    }
}

void UserAccountManager::addItem2FriendRequestList(std::shared_ptr<UserFriendRequest> info){
    m_friend_request_list.push_back(info);
}

const std::vector<std::shared_ptr<UserFriendRequest>> &UserAccountManager::getFriendRequestList()
{
    return m_friend_request_list;
}

bool UserAccountManager::alreadyExist(const QString &uuid) const
{
    auto it = std::find_if(m_friend_request_list.begin(), m_friend_request_list.end(),[uuid](std::shared_ptr<UserFriendRequest> item){
        /*uuid should equal to m_uuid(from_uuid)*/
        return item->m_uuid == uuid;
    });

    return it != m_friend_request_list.end();
}

void UserAccountManager::setUserInfo(std::shared_ptr<UserNameCard> info)
{
    m_userInfo = info;
}
