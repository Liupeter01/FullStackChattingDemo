/* be aware, messagenode.hpp needs c++17 */
#pragma once
#ifndef _MSGNODE_H_
#define _MSGNODE_H_
#include <algorithm>
#include <cstdint>
#include <optional>
#include <type_traits> //SFINAE
#include <utility>     // for std::declval

template <typename _Ty> struct add_const_lvalue_reference {
  using type = std::add_lvalue_reference_t<std::add_const_t<std::decay_t<_Ty>>>;
};

template <typename _Tp>
using add_const_lvalue_reference_t =
    typename add_const_lvalue_reference<_Tp>::type;

/* usually _Ty should be std::string or Qstring
 * but my point is if this container supports a iterator and size method
 * it should conpatiable with this library
 */
template <typename Container, typename = void>
struct send_msg_check : public std::false_type {};

template <typename Container>
struct send_msg_check<
    Container,
    std::void_t<decltype(std::declval<std::decay_t<Container>>().size()),
                decltype(std::declval<std::decay_t<Container>>().begin()),
                decltype(std::declval<std::decay_t<Container>>().end())>>
    : public std::true_type {};

/*creating a SFINAE to test wheather _Ty support append() member function*/
template <typename Container, typename = void>
struct recv_msg_check : public std::false_type {};

/*
 * 1. support begin/end iterator
 * 2. support append function
 *      append could both parameters which have copy ctor and copt ctor disabled
 * functions a) append(Container) b) append(const Container &)
 */
template <typename Container>
struct recv_msg_check<
    Container,
    std::void_t<decltype(std::declval<std::decay_t<Container>>().size()),
                decltype(std::declval<std::decay_t<Container>>()
                             .append(/*support copy ctor*/
                                     std::declval<std::decay_t<Container>>())),
                decltype(std::declval<std::decay_t<Container>>()
                             .append(/*copy ctor disabled*/
                                     std::declval<add_const_lvalue_reference_t<
                                         Container>>())),
                decltype(std::declval<std::decay_t<Container>>().data()),
                decltype(std::declval<std::decay_t<Container>>().begin()),
                decltype(std::declval<std::decay_t<Container>>().end())>>
    : public std::true_type {};

template <typename _Ty, typename = void> class RecvNode {
public:
  RecvNode() {
    static_assert(sizeof(_Ty) == 0,
                  "This type does not support append() member function");
  }
};

/*creating a SFINAE to test wheather _Ty support size() member function*/
template <typename _Ty, typename = void> class SendNode {
public:
  SendNode() {
    static_assert(
        sizeof(_Ty) == 0,
        "This type does not support size, begin and end member functions");
  }
};

template <typename Container> struct MessageNode {
  MessageNode() : _msg_id(0), _msg_length(0), _msg_data(0) {}

  uint16_t _msg_id;
  uint16_t _msg_length;
  Container _msg_data;
};

template <typename Container> struct MsgBody {
  /* default */
  MsgBody() : _msg(0) {}
  MsgBody(uint16_t max_length) : _msg(max_length) {}

  virtual ~MsgBody() {}
  Container _msg;
  static constexpr std::size_t HEADER_LENGTH =
      sizeof(uint16_t) + sizeof(uint16_t);
};

template <typename Container>
class RecvNode<Container, typename std::enable_if<
                              recv_msg_check<Container>::value, void>::type>
    : public MsgBody<Container> {

public:
  RecvNode() : MsgBody<Container>() {}

  std::optional<MessageNode<Container>> getMessageNode() {
    if (!isHeaderReady() || !isBodyReady()) {
      return std::nullopt;
    }
    /*
     * because data are ready, now invalid those data
     * and start to accept new datastream
     */
    _header_loading_status = false;
    _body_loading_status = false;
    return this->_node;
  }

  void insert_string(const Container &data) {
    /*receive data at first place*/
    this->_msg.append(data);

    /*there is still no data yet(because header is not ready)*/
    if (!isHeaderReady()) {
      if (this->_msg.size() < this->HEADER_LENGTH) {
        return;
      }
      check_header();
    }

    /* header processing is finished
     * moving to message body processing procedure!
     * if _msg.size() is larger than current msg size
     * then enable _header_loading_status
     */
    check_body();
  }

private:
  void check_header() {
    char *addr_msg_id = reinterpret_cast<char *>(this->_msg.data());
    this->_node._msg_id = *(reinterpret_cast<uint16_t *>(addr_msg_id));

    /*only store the valid message length, HEADER_LENGTH is not included*/
    char *addr_total_length = addr_msg_id + sizeof(uint16_t);
    this->_node._msg_length =
        *(reinterpret_cast<uint16_t *>(addr_total_length)) -
        this->HEADER_LENGTH;

    /*we have retrieved all header data, moving to parse body*/
    _header_loading_status = true;

    /*remove msg_id and length_info from buffer*/
    auto it = this->_msg.begin();
    std::advance(it, this->HEADER_LENGTH);

    this->_msg.erase(this->_msg.begin(), it);
  }

  void check_body() {
    if (this->_msg.size() >= this->_node._msg_length) {
      _header_loading_status = false;

      /*remove message_body inside _msg container*/
      auto it = this->_msg.begin();
      std::advance(it, this->_node._msg_length);

      /*copying data to new structure*/
      std::copy(this->_msg.begin(), it, _node._msg_data.begin());

      /*erase old message data*/
      this->_msg.erase(this->_msg.begin(), it);

      /*reenable loading status*/
      _header_loading_status = false;
      _body_loading_status = true;
    }
  }

  const bool isHeaderReady() const { return _header_loading_status; }
  const bool isBodyReady() const { return _body_loading_status; }

private:
  /*header id and header length is loaded or not*/
  bool _header_loading_status = false;
  bool _body_loading_status = false;

  /* -------------------------------------------------------
   * name | _msg_id | _total_length |         _msg         |
   * size |    2B   |       2B      |  _total_length - 4B  |
   * ------------------------------------------------------*/
  MessageNode<Container> _node;
};

template <typename Container>
class SendNode<Container, typename std::enable_if<
                              send_msg_check<Container>::value, void>::type> {
public:
  SendNode() : _node() {}

  /*
   * string.size() is the size of message body
   * the total length should also include the size of HEADER_LENGTH
   */
  SendNode(const Container &string, uint16_t msg_id) : _node() {
    /*set msg_id*/
    _node._msg_id = msg_id;

    /*set msg_length feild, MUST INCLUDE HEADER_LENGTH!*/
    _node._msg_length = string.size() + this->HEADER_LENGTH;
    _node._msg_data.append(string);
  }

  const auto getMessageID() const{return _node._msg_id;}
  const auto getTotalLenth() const{return _node._msg_length + HEADER_LENGTH;}
  const Container &getMessage() const { return _node._msg_data; }

private:
  /* -------------------------------------------------------
   * name | _msg_id | _total_length |         _msg         |
   * size |    2B   |       2B      |  _total_length - 4B  |
   * ------------------------------------------------------*/
  MessageNode<Container> _node;
    static constexpr std::size_t HEADER_LENGTH =
      sizeof(uint16_t) + sizeof(uint16_t);
};

#endif
