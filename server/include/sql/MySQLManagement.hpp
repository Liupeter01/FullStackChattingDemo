#pragma once
#ifndef _MYSQLMANAGEMENT_HPP_
#define _MYSQLMANAGEMENT_HPP_
#include<mutex>
#include<queue>
#include<thread>
#include<atomic>
#include<memory>
#include<optional>
#include<tools/tools.hpp>
#include<condition_variable>
#include<singleton/singleton.hpp>
#include<sql/MySQLConnection.hpp>

namespace mysql
{
          namespace details
          {
                    class MySQLManagement
                              :public Singleton<MySQLManagement>
                    {
                              friend class Singleton<MySQLManagement>;

                    public:
                              using context = mysql::MySQLConnection;
                              using context_ptr = std::unique_ptr<context>;

                              std::optional<context_ptr > acquire();
                              void release(context_ptr  conn);

                    public:
                              ~MySQLManagement();
                              void shutdown();

                    private:
                              MySQLManagement() noexcept;
                              MySQLManagement(std::size_t poolSize, 
                                                                  std::size_t timeOut,
                                                                  const std::string& username,
                                                                  const std::string& password,
                                                                  const std::string& database,
                                                                  const std::string& host = "localhost",
                                                                  const std::string& port = boost::mysql::default_port_string) noexcept;

                              void roundRobinChecking(std::size_t timeout);

                    private:
                              std::string m_username;
                              std::string m_password;
                              std::string m_database;
                              std::string m_host;
                              std::string m_port;

                              /*stop flag*/
                              std::atomic<bool> m_stop;

                              /*round-robin timeout check(second)*/
                              std::size_t m_timeout;

                              std::thread m_RRThread;

                              /*lock*/
                              std::mutex m_mtx;
                              std::condition_variable m_ctx;

                              /*queue*/
                              std::queue<context_ptr> m_queue;
                    };
          }

          class MySQLRAII 
          {
                    using wrapper = tools::ResourcesWrapper<details::MySQLManagement::context>;

          public:
                    MySQLRAII();
                    ~MySQLRAII();
                    std::optional<wrapper> operator->();

          private:
                    bool status; // load stub success flag
                    details::MySQLManagement::context_ptr m_mysql;
          };
}

#endif // !_MYSQLMANAGEMENT_HPP_
