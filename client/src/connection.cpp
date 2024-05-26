#include "connection.h"

namespace vchat {

std::thread* Connection::t = nullptr;
Connection* Connection::connection = nullptr;

Connection::Connection()
  : resolver(io), socket(io) {
  endpoint = resolver.resolve(address, port);
  LOG(INFO) << "start connect" << '\n';
  do_connect();
}

Connection* Connection::getinstance() {
  return new vchat::Connection();
}

void Connection::stop() {}

void Connection::do_connect() {
  async_connect(socket, endpoint,
    [&](boost::system::error_code ec, tcp::endpoint ep) {
      if (!ec) {
        LOG(INFO) << "connect successful" << '\n';
        do_readhead();
      } else {
        LOG(ERROR) << "connect error: "<< ec.what() << '\n';
      }
    }
  );
}

void Connection::do_readhead() {
  LOG(INFO) << "start read head" << '\n';
  std::shared_ptr<std::string> head = std::make_shared<std::string>(packer::getheadsize(), '\0');
  async_read(socket, boost::asio::buffer(*head, packer::getheadsize()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "read head successful" << '\n';
        Head head_ = packer::depackhead(head);
        LOG(INFO) << "start read body" << '\n';
        do_readbody(head_);
      } else {
        LOG(ERROR) << "read head error: "<< ec.what() << '\n';
      }
    }
  );
}

void Connection::do_readbody(Head head) {
  std::shared_ptr<std::string> body = std::make_shared<std::string>(packer::getheadsize(), '\0');
  async_read(socket, boost::asio::buffer(*body, head.size),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
        LOG(INFO) << "read body successful" << '\n';
        switch (head.method) {
          case login_success :
            Info::info->info_write([&]{
              Json::Value body_ = packer::depackbody(body);
              Info::info->userinfo.persionalinfo.id = body_["persionalinfo"]["id"].asInt();
              Info::info->userinfo.persionalinfo.password = body_["persionalinfo"]["password"].asInt();
              Info::info->userinfo.persionalinfo.username = body_["persionalinfo"]["username"].asInt();
              for(auto x : body_["friendlist"])
                Info::info->userinfo.friendlist.push_back(FriendInfo(x["id"].asInt()));
              for(auto x : body_["messagelist"])
                Info::info->userinfo.messagelist.push_back(
                  MessageInfo(x["sender"].asInt(),
                    x["receiver"].asInt(),
                    x["message"].asString()
                  )
                );
            });
            break;
          case chat_success :
            Info::info->info_write([&]{
              Json::Value body_ = packer::depackbody(body);
              MessageInfo temp;
              temp.sender = body_["sender"].asInt();
              temp.receiver = body_["receiver"].asInt();
              temp.msg = body_["message"].asString();
              Info::info->userinfo.messagelist.push_back(temp);
            });
            break;
          case addfriend_success :
            Info::info->info_write([&]{
              Json::Value body_ = packer::depackbody(body);
              FriendInfo temp;
              temp.friendid = body_["friendid"].asInt();
              Info::info->userinfo.friendlist.push_back(temp);
            });
            break;
        }
      }
      boost::asio::defer(io, [&]{ do_readhead(); });
    }
  );
}

void Connection::do_write(int status, Json::Value target) {
  LOG(INFO) << "start write" << '\n';
  std::string message = packer::enpack(status, target);
  async_write(socket, boost::asio::buffer(message, message.size()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
        LOG(INFO) << "write success" << '\n';
      }
  });
}

} // namespace vchat
