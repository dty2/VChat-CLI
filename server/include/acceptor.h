#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "connection.h"
#include <thread>

namespace vchat {
using boost::asio::ip::tcp;

class Acceptor {
public:
  static Acceptor* accept;

  Acceptor(const Acceptor&) = delete;
  Acceptor& operator=(const Acceptor&) = delete;
  static Acceptor* getInstance();
  //std::thread t;
  void run();

private:
  const std::string address = "127.0.0.1";
  const int port = 3864;
  boost::asio::io_context io;
  tcp::endpoint endpoint;
  tcp::acceptor acceptor;
  boost::asio::signal_set signals;
  ConnectionManager* connection_manager;

  Acceptor();
  void do_accept();
  void do_await_stop();

};

} // namespace vchat

#endif // ACCEPTOR_H
