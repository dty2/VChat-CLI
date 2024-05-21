#ifndef CONNECTION_H
#define CONNECTION_H

#include "work.h"

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
  static ConnectionManager* connection_manager;

  static ConnectionManager* getInstance(boost::asio::io_context&);
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;
  void start(connection_ptr);
  void stop(connection_ptr);
  void stop_all();

private:
  WorkManager* work_manager;
  explicit ConnectionManager(boost::asio::io_context&);
  std::set<connection_ptr> connections;

protected:
  boost::asio::io_context& io;
};

} // namespace vchat

#endif // CONNECTION_H

