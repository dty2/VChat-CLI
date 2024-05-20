#ifndef PACKAGE_H
#define PACKAGE_H

#include <array>
#include <string>
#include <variant>
#include <boost/asio/buffer.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace vchat {

// define method
enum Method {
  log_in = 1,
  log_out,
  sign_in,
  sign_out,
  add_friend,
  delete_friend,
  chat,
};

// define signal
enum Signal {
  success = 100,
  error_client = 300, // client error
  error_server = 400, // server error
};

// package
typedef struct Head {
  bool type; // false stand for ResponseHead, true stand for RequsetHead
  int size;
  std::variant<Method, Signal> operate;
} Head;

template<typename Type>
struct Package {
  Head head;
  Type body;
};

namespace Packer {
  template<typename Type>
  int getbodysize(Type);
  const int head_size = 16;
  const int body_size = 8192;
  /// enpack the message into a package
  template<typename Type>
  std::string enpack(Type, bool);
  /// depack the package into a message
  Head depack(std::array<char, head_size>&); // depack head
  template<typename Type>
  Type depack(std::array<char, body_size>&); // depack body
} // namespace packer

} // namespace vchat

#endif // PACKAGE_H
