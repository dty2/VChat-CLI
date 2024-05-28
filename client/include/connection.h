#ifndef CONNECTION_H
#define CONNECTION_H

#include "common.h"
#include "package.h"
#include "info.h"

namespace vchat {

class Function;
using boost::asio::ip::tcp;

class Connection {
public:
  boost::asio::io_context io;
  static Connection* connection;

  void stop();
  void do_write(int, Json::Value);
  ~Connection();
  static void getinstance();
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

private:
  static std::thread* t;
  char *head;
  char *body;
  Head hhead;
  const std::string port = "3864";
  const std::string address = "127.0.0.1";
  tcp::resolver resolver;
  tcp::resolver::results_type endpoint;
  tcp::socket socket;
  explicit Connection();

  void do_connect();
  void do_readhead();
  void do_readbody();

};

} // namespace vchat

#endif // CONNECTION_H
