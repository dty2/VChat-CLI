#include "connection.h"

namespace vchat {
using boost::asio::ip::tcp;

ConnectionManager::ConnectionManager(boost::asio::io_context& io_) : io(io_) {}

void ConnectionManager::start(connection_ptr cp) {
  connections.insert(cp);
  cp->start();
}

void ConnectionManager::stop(connection_ptr cp) {
  connections.erase(cp);
  cp->stop();
}

void ConnectionManager::stop_all() {
  for (auto temp: connections)
    temp->stop();
  connections.clear();
}

void Connection::start() { do_readhead(); }
void Connection::stop() { socket.close(); }
Connection::Connection( tcp::socket socket,
    ConnectionManager& connection_manager_, boost::asio::io_context& io_)
  : socket(std::move(socket)), connection_manager(connection_manager_), io(io_) {}

void Connection::do_readhead() {
  auto self(shared_from_this());
  async_read(socket, boost::asio::buffer(head, Packer::head_size),
    [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        Packer::depack(head);
        do_readbody();
      }
    }
  );
}

void Connection::do_readbody(int size) {
  auto self(shared_from_this());
  async_read(socket, boost::asio::buffer(buffer, size),
    [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
      }
      boost::asio::defer(io, [this]{ do_readhead(); });
    }
  );
}

void Connection::do_write() {
  auto self(shared_from_this());
  async_write(socket, boost::asio::buffer(),
    [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
      }
  });
}

} // namespace vchat
