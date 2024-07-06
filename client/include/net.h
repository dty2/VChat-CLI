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
  Net();
  ~Net();
  bool connect();
  void stop();
  bool write(int);
  bool write(int, Json::Value&);
  bool read(int&, Json::Value&);

private:
  const std::string port = "3864";
  const std::string address = "127.0.0.1";
  boost::asio::io_context io;
  tcp::socket socket;
  tcp::resolver resolver;
  tcp::resolver::results_type endpoint;
  char *head, *body;
  bool readhead(std::pair<int, int>&);
  bool readbody(int, Json::Value&);

};

} // namespace vchat

#endif // NET_H
