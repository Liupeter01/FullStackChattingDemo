#pragma once
#ifndef _SYNCLOGIC_HPP_
#define _SYNCLOGIC_HPP_
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <unordered_map>
#include <network/def.hpp>
#include <server/Session.hpp>
#include <condition_variable>
#include <singleton/singleton.hpp>

class SyncLogic 
          :public Singleton<SyncLogic>
{
          friend class Singleton<SyncLogic>;
          using CallbackFunc = std::function<void(ServiceType, std::shared_ptr<Session>, std::unique_ptr<RecvNode<std::string>>)>;

public:
          using SessionPtr = std::shared_ptr<Session>;
          using NodePtr = std::unique_ptr<RecvNode<std::string>>;
          using pair = std::pair<SessionPtr, NodePtr>;

public:
          ~SyncLogic();
          void commit(pair recv_node);

private:
          SyncLogic();

          /*SyncLogic Class Operations*/
          void shutdown();
          void processing();
          void registerCallbacks();
          void execute(pair &&node);

          /*Execute Operations*/
          void handlingLogin(ServiceType srv_type, std::shared_ptr<Session> session, std::unique_ptr<RecvNode<std::string>> recv);
          void handlingLogout(ServiceType srv_type, std::shared_ptr<Session> session, std::unique_ptr<RecvNode<std::string>> recv);

private:
          std::atomic<bool> m_stop;

          /*working thread, handling commited request*/
          std::thread m_working;

          /*mutex & cv => thread safety*/
          std::mutex m_mtx;
          std::condition_variable m_cv;

          /*user commit data to the queue*/
          std::queue< pair> m_queue;

          /*callback list*/
          std::unordered_map<ServiceType, CallbackFunc> m_callbacks;
};

#endif _SYNCLOGIC_HPP_