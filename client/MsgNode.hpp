/* be aware, messagenode.hpp needs c++17 */
#pragma once
#include <iterator>
#ifndef _MSGNODE_H_
#define _MSGNODE_H_
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <type_traits> //SFINAE
#include <utility>     // for std::declval

class QString;

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
    std::void_t<typename Container::iterator,
                decltype(std::declval<std::decay_t<Container>>().size()),
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
    std::void_t<typename Container::iterator,
                decltype(std::declval<std::decay_t<Container>>().size()),
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

template <typename Container> struct MsgHeader {
  /*For receiving data*/
  MsgHeader() noexcept : _id(0), _length(HEADER_LENGTH), _cur_length(0) {
    _buffer.resize(_length, 0);
  }

  /*for sending message*/
  MsgHeader(uint16_t id, const Container &input) noexcept
      : _id(id), _length(input.size() + HEADER_LENGTH), _cur_length(0) {
    _buffer.resize(_length, 0);
  }

  /* ---------------------------------------------
   * name |  _id  |  _length  |     _buffer      |
   * size |   2B  |     2B    |   _length - 4B   |
   * --------------------------------------------*/
  uint16_t _id;
  uint16_t _length; /*total length*/
  uint16_t _cur_length;
  Container _buffer;

  static constexpr std::size_t HEADER_LENGTH =
      sizeof(uint16_t) + sizeof(uint16_t);

  static constexpr std::size_t get_header_length() { return HEADER_LENGTH; }

  typename std::iterator_traits<typename Container::iterator>::pointer
  get_header_base() {
    return _buffer.data();
  }
  typename std::iterator_traits<typename Container::iterator>::pointer
  get_body_base() {
    return get_header_base() + HEADER_LENGTH;
  }

  /*please be aware, it should be network sequence, please convert network
   * sequence to host*/
  std::optional<uint16_t> get_id() {
    if (!check_header_remaining()) { /*not OK*/
      return std::nullopt;
    }
    _id = *(reinterpret_cast<uint16_t *>(get_id_base()));
    return _id;
  }

  /*
   * when user deploy gen_length, it will ONLY return the size of message!
   * please be aware, it should be network sequence, please convert network
   * sequence to host
   */
  std::optional<uint16_t> get_length() {
    if (!check_header_remaining()) { /*not OK*/
      return std::nullopt;
    }

    uint16_t total_length = *(reinterpret_cast<uint16_t *>(get_length_base()));

    /*we only have the header length*/
    if (_length < total_length) {
      _length = total_length;

      /*extend the size of buffer*/
      _buffer.resize(_length, 0);
    }
    return _length - HEADER_LENGTH;
  }
  std::optional<Container> get_msg_body() {
    if (!check_body_remaining()) {
      return std::nullopt;
    }

    /*unfortunely, QByteArray does not support assign
     * QString is an exception
     * auto ib = _buffer.begin();
     * auto ie = _buffer.begin();
     * std::advance(ib, HEADER_LENGTH);
     * std::advance(ie, _length);
     * Container ret;
     * ret.assign(ib, ie);
     */
    if constexpr (std::is_same_v<Container, QString>) {
      return this->_buffer.mid(HEADER_LENGTH, _length);
    }
    return Container(get_body_base(), _length - HEADER_LENGTH);
  }
  void update_pointer_pos(const uint16_t increment) {
    _cur_length += increment;
  }
  void clear() {
    /*clean previous packet*/
    auto ie = _buffer.begin();
    std::advance(ie, _length);
    _buffer.erase(_buffer.begin(), ie);

    /*clear all the data*/
    _cur_length = 0;
    _length = 0;
  }
  bool check_header_remaining() const {
    return !(_cur_length >= HEADER_LENGTH);
  }
  bool check_body_remaining() const { return !(_cur_length >= _length); }

private:
  typename std::iterator_traits<typename Container::iterator>::pointer
  get_id_base() {
    return get_header_base();
  }
  typename std::iterator_traits<typename Container::iterator>::pointer
  get_length_base() {
    return get_header_base() + sizeof(uint16_t);
  }
};

template <typename Container>
class RecvNode<Container, typename std::enable_if<
                              recv_msg_check<Container>::value, void>::type>
    : public MsgHeader<Container> {
public:
  RecvNode() noexcept : MsgHeader<Container>() {}
  bool insert_header(const Container &data) {
    this->_buffer.append(data);

    /*increment current pointer*/
    auto length = data.size();
    this->update_pointer_pos(length);

    /*header is complete or not*/
    return this->get_length().has_value();
  }

  bool insert_body(const Container &data) {
    this->_buffer.append(data);

    /*increment current pointer*/
    auto length = data.size();
    this->update_pointer_pos(length);

    return this->get_msg_body().has_value();
  }
};

template <typename Container>
struct SendNode<Container, typename std::enable_if<
                               send_msg_check<Container>::value, void>::type>
    : public MsgHeader<Container> {

  template <typename _Callable>
  SendNode(uint16_t msg_id, Container &string, _Callable &&convert) noexcept
      : MsgHeader<Container>(msg_id, string) {

    auto cv_id = convert(this->_id);
    auto cv_length = convert(this->_length);

    if constexpr (std::is_same_v<Container, std::string>) {
      this->_buffer.append(std::to_string(cv_id));
      this->_buffer.append(std::to_string(cv_length));
    } else {
      this->_buffer.append(Container::number(cv_id));
      this->_buffer.append(Container::number(cv_length));
    }
    for (auto &ch : string) {
      ch = static_cast<typename std::iterator_traits<
          typename Container::iterator>::value_type>(convert(ch));
    }

    /**/
    this->_buffer.append(string);
  }

  const std::size_t get_full_length() const { return this->_length; }
};

#endif
