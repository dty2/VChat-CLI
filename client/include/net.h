#ifndef NET_H
#define NET_H

#include "common.h"
#include "package.h"

#define HEAD_SIZE 8
#define BODY_SIZE 50000
#define ALL_SIZE 50008

namespace vchat {

using boost::asio::ip::tcp;

class Net {
public:
  Net(std::function<void(int, Json::Value)>);
  ~Net();
  void start();
  void close();
  bool write(int);
  bool write(int, Json::Value&);

private:
  std::function<void(int, Json::Value)> handle;
  const std::string port = "3784";
  const std::string address = "127.0.0.1";
  boost::asio::io_context io;
  tcp::socket socket;
  tcp::resolver resolver;
  tcp::resolver::results_type endpoint;
  char *head, *body;
  void readhead();
  void readbody(std::pair<int, int>);

};

} // namespace vchat

#endif // NET_H
