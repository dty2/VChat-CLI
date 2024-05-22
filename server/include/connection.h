#ifndef CONNECTION_H
#define CONNECTION_H

#include "work.h"

namespace vchat {
using boost::asio::ip::tcp;

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection> {
public:
  explicit Connection(tcp::socket, ConnectionManager*);
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  void start();
  void stop();

private:
  ConnectionManager* connection_manager;
  tcp::socket socket;
  void do_readhead();
  void do_readbody(Head);
  void do_write(int, Json::Value);
  void do_chat(int, Json::Value);
  void update(int);
};
typedef std::shared_ptr<Connection> connection_ptr;

class ConnectionManager {
public:
 // static ConnectionManager* connection_manager;

  static ConnectionManager* getInstance(boost::asio::io_context&);
  ConnectionManager(const ConnectionManager&) = delete;
  ConnectionManager& operator=(const ConnectionManager&) = delete;
  void start(connection_ptr);
  void stop(connection_ptr);
  void stop_all();

private:
  WorkManager* work_manager;
  explicit ConnectionManager(boost::asio::io_context&);
  friend Connection;

protected:
  std::unordered_map<connection_ptr, int> connections;
  boost::asio::io_context& io;
};

extern ConnectionManager* connection_manager;

} // namespace vchat

#endif // CONNECTION_H

