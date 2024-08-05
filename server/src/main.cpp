#include<iostream>
#include<config/ServerConfig.hpp>
#include<server/GateServer.hpp>

int main() 
{
		  try
		  {
					boost::asio::io_context ioc;
					std::shared_ptr<GateServer> server = std::make_shared<GateServer>(
							  ioc, ServerConfig::get_instance()->GateServerPort
					);
					server->serverStart();

					/*setting up signal*/
					boost::asio::signal_set signal{ ioc, SIGINT, SIGTERM };
					signal.async_wait([&ioc](boost::system::error_code ec, int sig_number) {
							  if (ec) {
										return;
							  }
							  ioc.stop();
					});
					ioc.run();
		  }
		  catch (const std::exception& e){
					std::cerr << e.what() << '\n';
		  }
          return 0;
}

