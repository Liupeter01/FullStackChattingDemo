#pragma once
#ifndef _STUBPOOL_HPP_
#define _STUBPOOL_HPP_
#include<queue>
#include<mutex>
#include<optional>
#include<condition_variable>

#include<tools/tools.hpp>
#include<grpcpp/grpcpp.h>
#include<message/message.grpc.pb.h>
#include<singleton/singleton.hpp>

namespace stubpool 
{
		  namespace details 
		  {
					class StubPool :public Singleton<StubPool>
					{
							  friend class Singleton<StubPool>;
							  StubPool();

					public:
							  using stub = message::VerificationService::Stub;
							  using stub_ptr = std::unique_ptr< stub>;

							  ~StubPool();
							  void shutdown();

							  std::optional<stub_ptr> acquire();
							  void release(stub_ptr stub);

					private:
							  /*Stubpool stop flag*/
							  std::atomic<bool> m_stop;

							  /*Stub Ammount*/
							  std::size_t m_queue_size;

							  /*record address info and credentials*/
							  grpc::string m_addr;
							  std::shared_ptr<grpc::ChannelCredentials> m_cred;

							  /*queue control*/
							  std::mutex m_mtx;
							  std::condition_variable m_cv;

							  /*stub queue*/
							  std::queue< stub_ptr> m_stub_queue;
					};
		  }

		  /*
		  * get stub automatically!
		  */
		  class StubRAII {
					using wrapper = tools::ResourcesWrapper< details::StubPool::stub>;

		  public:
					StubRAII();
					~StubRAII();
					std::optional<wrapper> operator->();

		  private:
					bool status;		  //load stub success flag
					details::StubPool::stub_ptr m_stub;
		  };
}

#endif // !_STUBPOOL_HPP_