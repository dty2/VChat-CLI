#include "connection.h"
#include "package.h"
#include "server.h"
#include "service.h"

using boost::asio::ip::tcp;

ConnectionManager::ConnectionManager() {}

void ConnectionManager::add(Connection_ptr cp) {
  connections.insert(cp);
  cp->start();
}

void ConnectionManager::stop(Connection_ptr cp) {
  connections.erase(cp);
  cp->stop();
}

void ConnectionManager::stop_all() {
  for(auto& v : connections)
    v->stop();
  connections.clear();
}

Connection::Connection(tcp::socket socket_, ConnectionManager* manager_)
  : socket(std::move(socket_)), manager(manager_) {
  head = new char[HEAD_SIZE];
  body = new char[BODY_SIZE];
}

Connection::~Connection() {
  free(head);
  free(body);
}

void Connection::start() { readhead(); }

void Connection::stop() { socket.close(); }

void Connection::readhead() {
  async_read(socket, boost::asio::buffer(head, HEAD_SIZE),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        headinfo = packer::depackhead(head);
        readbody();
      } else {
        this->stop();
        manager->connections.erase(shared_from_this());
      }
    }
  );
}

void Connection::readbody() {
  async_read(socket, boost::asio::buffer(body, headinfo.second),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
        Json::Value value = packer::depackbody(body, headinfo.second);
        Service::serve(headinfo.first, std::move(value), this);
      }
      readhead();
    }
  );
}

void Connection::write(int status, Json::Value target) {
  std::string message = packer::enpack(status, target);
  char *msg = new char[ALL_SIZE];
  std::copy(message.begin(), message.end(), msg);
  async_write(socket, boost::asio::buffer(msg, message.size()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
      } else {
        this->stop();
        manager->connections.erase(shared_from_this());
      }
  });
}
