#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include <json/value.h>
#include <set>
#include <memory>
#include <string>

#include "package.h"

namespace vchat {
using boost::asio::ip::tcp;

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection> {
public:
  Connection(tcp::socket, ConnectionManager&, boost::asio::io_context&); 
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  void start();
  void stop();

private:
  boost::asio::io_context& io;
  tcp::socket socket;
  ConnectionManager& connection_manager;
  void do_readhead();
  void do_readbody(Head);
  void do_write(int, Json::Value);
};
typedef std::shared_ptr<Connection> connection_ptr;

class ConnectionManager {
public:
  explicit ConnectionManager(boost::asio::io_context&);
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;
  void start(connection_ptr);
  void stop(connection_ptr);
  void stop_all();

private:
  std::set<connection_ptr> connections;

protected:
  boost::asio::io_context& io;

};

} // namespace vchat

#endif // CONNECTION_H

