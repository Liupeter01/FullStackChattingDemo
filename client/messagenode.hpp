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
  struct MessageNode {
    uint16_t _msg_id;
    uint16_t _msg_length;
    Container _msg_data;
  };

  RecvNode() : MsgBody<Container>() {}

  std::optional<MessageNode> getMessageNode() {
    if (isHeaderReady() && isBodyReady()) {
      /*
       * because data are ready, now invalid those data
       * and start to accept new datastream
       */
      _header_loading_status = false;
      _body_loading_status = false;
      return this->_node;
    }
    return std::nullopt;
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
    this->_node._msg_id = *(reinterpret_cast<uint16_t *>(
        reinterpret_cast<char *>(this->_msg.data())));

    this->_node._msg_length = *(reinterpret_cast<uint16_t *>(
        reinterpret_cast<char *>(this->_msg.data()) + sizeof(uint16_t)));

    /*we have retrieved all header data*/
    _header_loading_status = true;

    /*remove msg_id and length_info inside _msg container*/
    auto it = this->_msg.begin();
    std::advance(it, this->HEADER_LENGTH);
    this->_msg.erase(this->_msg.begin(), it);
  }

  void check_body() {
    if (this->_msg.size() >= this->_node._total_length - this->HEADER_LENGTH) {
      _header_loading_status = false;

      /*remove message_body inside _msg container*/
      auto it = this->_msg.begin();
      std::advance(it, this->_node._total_length - this->HEADER_LENGTH);

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
  MessageNode _node;
};

template <typename Container>
class SendNode<Container, typename std::enable_if<
                              send_msg_check<Container>::value, void>::type>
    : public MsgBody<Container> {

public:
  SendNode(const Container &string, uint16_t msg_id)
      : _msg_id(msg_id), _total_length(string.size() + this->HEADER_LENGTH),
        MsgBody<Container>(string.size() + this->HEADER_LENGTH) {

    /*when sending message, the cur_length = string.size() */
    std::copy(string.begin(), string.end(), this->_msg.begin());
  }

  const uint16_t getMsgID() const { return _msg_id; }

private:
  /* -------------------------------------------------------
   * name | _msg_id | _total_length |         _msg         |
   * size |    2B   |       2B      |  _total_length - 4B  |
   * ------------------------------------------------------*/
  uint16_t _msg_id;
  uint16_t _total_length;
};

#endif
