#include<useraccountmanager.hpp>

const std::vector<std::shared_ptr<UserFriendRequest>> &UserAccountManager::getFriendRequestList()
{
    return m_friend_request_list;
}

void UserAccountManager::add2FriendRequestList(std::shared_ptr<UserFriendRequest> item)
{
    m_friend_request_list.push_back(item);
}

bool UserAccountManager::alreadyExist(const QString &uuid) const
{
    auto it = std::find_if(m_friend_request_list.begin(), m_friend_request_list.end(),[uuid](std::shared_ptr<UserFriendRequest> item){
        /*uuid should equal to m_uuid(from_uuid)*/
        return item->m_uuid == uuid;
    });

    return it != m_friend_request_list.end();
}
