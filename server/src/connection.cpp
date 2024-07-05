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

Connection::Connection(tcp::socket socket_, ConnectionManager* connection_manager_)
  : socket(std::move(socket_)), connection_manager(connection_manager_) {
  chead = new char[9];
  body = new char[50000];
}
Connection::~Connection() {
  free(chead);
  free(body);
}

void Connection::start() { do_readhead(); }

void Connection::update(int id) {
  connection_manager->connections[shared_from_this()] = id;
}

void Connection::stop() { socket.close(); }


void Connection::do_readhead() {
  LOG(INFO) << "start read head" << '\n';
  async_read(socket, boost::asio::buffer(chead, 9),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "deal with chead " << '\n';
        LOG(INFO) << "read size: " << bytes_transferred << '\n';
        //for(int i = 0; i < 9; i ++)
        //  LOG(INFO) << head[i];
        hhead = packer::depackhead(chead);
        LOG(INFO) << hhead.size << ' ' << hhead.type << ' ' << hhead.method;
        do_readbody();
      } else {
        LOG(INFO) << connection_manager->connections[shared_from_this()] << "log out";
        this->stop();
        connection_manager->connections.erase(shared_from_this());
      }
    }
  );
}

void Connection::do_readbody() {
  LOG(INFO) << "start read body";
  async_read(socket, boost::asio::buffer(body, hhead.size),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
        LOG(INFO) << "deal with body";
        LOG(INFO) << "read size: " << bytes_transferred << '\n';
        WorkManager::push_work(hhead, packer::depackbody(body, hhead.size), 
          std::bind(&Connection::do_write, this, std::placeholders::_1, std::placeholders::_2)
        );
        LOG(INFO) << "push_work finish";
      }
      boost::asio::defer(connection_manager->io, [&]{ do_readhead(); });
    }
  );
}

void Connection::do_addfriend(int status, Json::Value target) {
  auto self(shared_from_this());
  int receiver = target["friendid"].asInt();
  std::string message = packer::enpack(status, target);
  for(auto& [item, id] : connection_manager->connections)
    if(id == receiver) {
      async_write(item->socket, boost::asio::buffer(message, message.size()),
        [&](boost::system::error_code ec, std::size_t bytes_transferred) {
          if (!ec) {
            LOG(INFO) << "add friend successful";
          } else {
            LOG(INFO) << connection_manager->connections[shared_from_this()] << "log out";
            this->stop();
            connection_manager->connections.erase(shared_from_this());
          }
        }
      );
    }
  async_write(socket, boost::asio::buffer(message, message.size()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "add friend successful";
      } else {
        LOG(INFO) << connection_manager->connections[shared_from_this()] << "log out";
        this->stop();
        connection_manager->connections.erase(shared_from_this());
      }
    }
  );
}

void Connection::do_chat(int status, Json::Value target) {
  auto self(shared_from_this());
  int receiver = target["receiver"].asInt();
  std::string message = packer::enpack(status, target);
  for(auto& [item, id] : connection_manager->connections)
    if(id == receiver) {
      async_write(item->socket, boost::asio::buffer(message, message.size()),
        [&](boost::system::error_code ec, std::size_t bytes_transferred) {
          if (!ec) {
            LOG(INFO) << "send successful" << '\n';
          } else {
            LOG(INFO) << connection_manager->connections[shared_from_this()] << "log out";
            this->stop();
            connection_manager->connections.erase(shared_from_this());
          }
        }
      );
    }
  async_write(socket, boost::asio::buffer(message, message.size()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "send successful" << '\n';
      } else {
        LOG(INFO) << connection_manager->connections[shared_from_this()] << "log out";
        this->stop();
        connection_manager->connections.erase(shared_from_this());
      }
    }
  );
}

void Connection::do_write(int status, Json::Value target) {
  if(status == chat_success) { do_chat(status, target); }
  else if(status == addfriend_success) { do_addfriend(status, target); }
  else {
    LOG(INFO) << "do write";
    std::string message = packer::enpack(status, target);
    LOG(INFO) << "message :" << message << '\n';
    if(status == login_success) {
      LOG(INFO) << target["id"] << "log in";
      update(target["id"].asInt());
    }
    char *msg = new char[50009];
    std::copy(message.begin(), message.end(), msg);
    async_write(socket, boost::asio::buffer(msg, message.size()),
      [&](boost::system::error_code ec, std::size_t bytes_transferred) {
        if (!ec) {
          LOG(INFO) << "write successful" << '\n';
        } else {
          LOG(INFO) << connection_manager->connections[shared_from_this()] << "log out";
          this->stop();
          connection_manager->connections.erase(shared_from_this());
        }
    });
  }
}

} // namespace vchat
