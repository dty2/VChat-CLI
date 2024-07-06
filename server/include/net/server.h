#ifndef SERVER_H
#define SERVER_H

#include "connection.h"

#define HEAD_SIZE 8
#define BODY_SIZE 50000
#define ALL_SIZE 50008

using boost::asio::ip::tcp;

class Server {
public:
  Server(const Server&) = delete;
  Server& operator=(const Server&) = delete;
  explicit Server();
  void run();

private:
  const std::string address = "127.0.0.1";
  const int port = 3864;
  tcp::endpoint endpoint;
  tcp::acceptor acceptor;
  void do_accept();
  void do_await_stop();
  boost::asio::io_context io_context;
  boost::asio::signal_set signals;
  ConnectionManager connection_manager;

};

#endif // !SERVER_H
