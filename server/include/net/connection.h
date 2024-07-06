#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::tcp;

class Service;
class Connection;
typedef std::shared_ptr<Connection> Connection_ptr;

class ConnectionManager {
public:
  ConnectionManager();
  void add(Connection_ptr);
  void stop(Connection_ptr);
  void stop_all();
  std::set<Connection_ptr> connections;
  friend Connection;
};

class Connection : public std::enable_shared_from_this<Connection> {
public:
  Connection(tcp::socket, ConnectionManager*);
  ~Connection();
  void start();
  void stop();
  void write(int, Json::Value);

private:
  ConnectionManager* manager;
  tcp::socket socket;
  char *head, *body;
  void readhead();
  void readbody(std::pair<int, int>);
};

#endif //CONNECTION_H

