#pragma once
#ifndef _HANDLEMETHOD_HPP_
#define _HANDLEMETHOD_HPP_
#include<map>
#include<memory>
#include<functional>
#include<string_view>
#include<singleton/singleton.hpp>

class HTTPConnection;

class HandleMethod
          :public Singleton<HandleMethod>
{
          friend class Singleton<HandleMethod>;
          using  CallBackFunc = std::function<void(std::shared_ptr<HTTPConnection>)>;

private:
          HandleMethod();
          void registerCallBacks();

public:
          ~HandleMethod();
          bool handleGetMethod(std::string str, std::shared_ptr<HTTPConnection> extended_lifetime);

private:
          /*CallBack Functions*/
          std::map</*url*/std::string, CallBackFunc> get_method_callback;
          std::map</*url*/std::string, CallBackFunc> post_method_callback;
};

#endif