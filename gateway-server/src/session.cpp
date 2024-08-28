#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <server/session.hpp>

Session::Session(boost::asio::io_context &_ioc, GateServer *my_gate)
    : s_socket(_ioc), s_gate(my_gate) {
  /*generate uuid string*/
  boost::uuids::uuid uuid_gen = boost::uuids::random_generator()();
  this->s_uuid = boost::uuids::to_string(uuid_gen);
}
