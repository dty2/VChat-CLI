#include "connection.h"

namespace vchat {

void Connection::stop() {}

Connection::Connection()
  : resolver(io), socket(io) {
  endpoint = resolver.resolve(address, port);
}

Connection* Connection::getinstance() {
  Connection* instance = new vchat::Connection();
  return instance;
}

void Connection::do_connect() {
  async_connect(socket, endpoint,
    [&](boost::system::error_code ec, tcp::endpoint) {
      if (!ec) {
        LOG(INFO) << "start connect" << '\n';
        do_readhead();
      }
    });
}
void Connection::do_readhead() {
  auto self(shared_from_this());
  std::shared_ptr<std::string> head = std::make_shared<std::string>();
  async_read(socket, boost::asio::buffer(*head, packer::getheadsize()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "start read head" << '\n';
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
        LOG(INFO) << "start read body" << '\n';
      }
      boost::asio::defer(io, [&]{ do_readhead(); });
    }
  );
}

void Connection::do_write(int status, Json::Value target) {
  auto self(shared_from_this());
  std::string message = packer::enpack(status, target);
  async_write(socket, boost::asio::buffer(message, message.size()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
      }
  });
}

} // namespace vchat
