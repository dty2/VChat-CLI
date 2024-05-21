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
  std::shared_ptr<std::string> head = std::make_shared<std::string>();
  auto self(shared_from_this());
  async_read(socket, boost::asio::buffer(*head, packer::getheadsize()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "read head" << '\n';
        Head head_ = packer::depackhead(head);
        do_readbody(head_);
      }
    }
  );
}

void Connection::do_readbody(Head head) {
  auto self(shared_from_this());
  std::shared_ptr<std::string> body = std::make_shared<std::string>();
  async_read(socket, boost::asio::buffer(*body, head.size),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
        LOG(INFO) << "read body" << '\n';
      }
      boost::asio::defer(io, [&]{ do_readhead(); });
    }
  );
}

void Connection::do_write(int method, Json::Value target) {
  auto self(shared_from_this());
  std::string message = packer::enpack(method, target);
  async_write(socket, boost::asio::buffer(message, message.size()),
    [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "send message successful" << '\n';
      }
  });
}

} // namespace vchat
