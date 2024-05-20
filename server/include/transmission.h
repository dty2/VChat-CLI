#ifndef TRANSMISSION_H
#define TRANSMISSION_H

#include <boost/asio.hpp>
#include "connection.h"

class Transmission;

namespace vchat {
using boost::asio::ip::tcp;

class Transmission {
public:
  Transmission(const Transmission&) = delete;
  Transmission& operator=(const Transmission&) = delete;
  static Transmission* getInstance();
  void run();
private:
  const std::string address = "127.0.0.1";
  const int port = 3000;
  static Transmission* transmission;
  boost::asio::io_context io;
  tcp::endpoint endpoint;
  tcp::acceptor acceptor;
  boost::asio::signal_set signals;
  ConnectionManager connection_manager;

  Transmission();
  void do_accept();
  void do_await_stop();

};

} // namespace vchat

#endif // TRANSMISSION_H
