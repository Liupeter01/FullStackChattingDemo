#pragma once
#ifndef _HANDLEMETHOD_HPP_
#define _HANDLEMETHOD_HPP_
#include<map>
#include<memory>
#include<functional>
#include<string_view>
#include <network/def.hpp> //network errorcode defs
#include<singleton/singleton.hpp>

class HTTPConnection;

class HandleMethod
          :public Singleton<HandleMethod>
{
          friend class Singleton<HandleMethod>;
          using  CallBackNoReturn = std::function<void(std::shared_ptr<HTTPConnection>)>;
          using CallBackWithStatus = std::function<bool(std::shared_ptr<HTTPConnection>)>;

private:
          HandleMethod();
          void registerGetCallBacks();
          void registerPostCallBacks();

          void generateErrorMessage(std::string_view message, 
                                                        ServiceStatus status, 
                                                         std::shared_ptr<HTTPConnection> conn);

public:
          ~HandleMethod();
          void registerCallBacks();
          bool handleGetMethod(std::string str, std::shared_ptr<HTTPConnection> extended_lifetime);
          bool handlePostMethod(std::string str, std::shared_ptr<HTTPConnection> extended_lifetime);

private:
          /*CallBack Functions*/
          std::map</*url*/std::string, CallBackNoReturn> get_method_callback;
          std::map</*url*/std::string, CallBackWithStatus> post_method_callback;
};

#endif