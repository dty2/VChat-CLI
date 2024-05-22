#ifndef CONNECTION_H
#define CONNECTION_H

#include "common.h"
#include "package.h"

namespace vchat {
using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection> {
public:
  boost::asio::io_context io;

  void do_connect();
  static Connection* getinstance();
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  void stop();

private:
  const std::string port = "3000";
  const std::string address = "127.0.0.1";
  tcp::resolver resolver;
  tcp::resolver::results_type endpoint;
  tcp::socket socket;
  explicit Connection();

  void do_readhead();
  void do_readbody(Head);
  void do_write(int, Json::Value);

};

} // namespace vchat

#endif // CONNECTION_H
