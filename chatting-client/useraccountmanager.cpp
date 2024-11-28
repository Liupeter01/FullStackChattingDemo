#include <QJsonArray>
#include<useraccountmanager.hpp>

void UserAccountManager::appendFriendRequestList(const QJsonArray& array){
    for(const  QJsonValue &obj : array){
        addItem2List(
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

void UserAccountManager::appendAuthFriendList(const QJsonArray &array){
    for(const  QJsonValue &obj : array){
       addItem2List(
            std::make_shared<UserNameCard>(
                obj["uuid"].toString(),
                obj["avator"].toString(),
                obj["username"].toString(),
                obj["nickname"].toString(),
                obj["description"].toString(),
                static_cast<Sex>(obj["sex"].toInt())
        ));
    }
}

void UserAccountManager::appendArrayToList(TargetList target, const QJsonArray &array){
    if(target == TargetList::FRIENDLIST){
        appendAuthFriendList(array);
    }
    else if(target == TargetList::REQUESTLIST){
        appendFriendRequestList(array);
    }
}

void UserAccountManager::addItem2List(std::shared_ptr<UserFriendRequest> info){
    m_friend_request_list.push_back(info);
}

void UserAccountManager::addItem2List(std::shared_ptr<UserNameCard> info){
    m_auth_friend_list[info->m_uuid] = info;
}

const std::vector<std::shared_ptr<UserFriendRequest>> &UserAccountManager::getFriendRequestList(){
    return m_friend_request_list;
}

std::vector<std::shared_ptr<UserNameCard>> UserAccountManager::getAuthFriendList(){
    std::vector<std::shared_ptr<UserNameCard>> list;
    list.reserve(m_auth_friend_list.size());    //reserve size

    for(const auto &[key, value]: m_auth_friend_list){
        list.push_back(value);
    }
    return list;
}

bool UserAccountManager::alreadyExistInAuthList(const QString &uuid) const{
    return m_auth_friend_list.find(uuid) != m_auth_friend_list.end();
}

bool UserAccountManager::alreadyExistInRequestList(const QString &uuid) const{
    auto it = std::find_if(m_friend_request_list.begin(), m_friend_request_list.end(),[uuid](std::shared_ptr<UserFriendRequest> item){
        //uuid should equal to m_uuid(from_uuid)
        return item->m_uuid == uuid;
    });

    return it != m_friend_request_list.end();
}

void UserAccountManager::setUserInfo(std::shared_ptr<UserNameCard> info){
    m_userInfo = info;
}
