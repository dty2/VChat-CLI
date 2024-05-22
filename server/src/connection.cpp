#include "connection.h"

namespace vchat {
using boost::asio::ip::tcp;

ConnectionManager* ConnectionManager::getInstance(boost::asio::io_context& io_) {
  ConnectionManager* instance = new ConnectionManager(io_);
  return instance;
}

ConnectionManager::ConnectionManager(boost::asio::io_context& io_) : io(io_) {
  work_manager = WorkManager::getInstance();
}

void ConnectionManager::start(connection_ptr cp) {
  connections[cp] = -1;
  cp->start();
}

void ConnectionManager::stop(connection_ptr cp) {
  connections.erase(cp);
  cp->stop();
}

void ConnectionManager::stop_all() {
  for(auto& [item, id] : connections)
    item->stop();
  connections.clear();
}

void Connection::start() { do_readhead(); }

void Connection::update(int id) {
  auto self(shared_from_this());
  connection_manager->connections[self] = id;
}

void Connection::stop() { socket.close(); }
Connection::Connection(tcp::socket socket, ConnectionManager* connection_manager_)
  : socket(std::move(socket)), connection_manager(connection_manager_) {}

void Connection::do_readhead() {
  std::shared_ptr<std::string> head = std::make_shared<std::string>();
  auto self(shared_from_this());
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
        WorkManager::push_work(head, packer::depackbody(body), 
          std::bind(&Connection::do_write, this, std::placeholders::_1, std::placeholders::_2)
        );
      }
      boost::asio::defer(connection_manager->io, [&]{ do_readhead(); });
    }
  );
}

void Connection::do_chat(int status, Json::Value target) {
  auto self(shared_from_this());
  int receiver = target["receiver"].asInt();
  for(auto& [item, id] : connection_manager->connections)
    if(id == receiver) {
      std::string message = packer::enpack(status, target);
      async_write(item->socket, boost::asio::buffer(message, message.size()),
        [&](boost::system::error_code ec, std::size_t bytes_transferred) {
          if (!ec) {
            LOG(INFO) << "send message successful" << '\n';
            if(status == login_success) {
              update(target["id"].asInt());
            } else {
              this->stop();
              connection_manager->connections.erase(self);
            }
          }
        }
      );
    }
}

void Connection::do_write(int status, Json::Value target) {
  auto self(shared_from_this());
  if(status == chat_success) { do_chat(status, target); }
  else {
    std::string message = packer::enpack(status, target);
    async_write(socket, boost::asio::buffer(message, message.size()),
      [&](boost::system::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          LOG(INFO) << "send successful" << '\n';
          if(status == login_success) {
            update(target["id"].asInt());
          } else {
            this->stop();
            connection_manager->connections.erase(self);
          }
        }
    });
  }
}

} // namespace vchat
