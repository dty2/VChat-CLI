#include "connection.h"

namespace vchat {

std::thread* Connection::t = nullptr;
Connection* Connection::connection = nullptr;

Connection::Connection() : resolver(io), socket(io) {
  endpoint = resolver.resolve(address, port);
  head = new char[9];
  body = new char[50000];
  LOG(INFO) << "start connect" << '\n';
  do_connect();
}

Connection::~Connection() {
  free(head);
  free(body);
}

void Connection::getinstance() {
  connection = new vchat::Connection();
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
  async_read(socket, boost::asio::buffer(head, 9),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        LOG(INFO) << "read head successful" << '\n';
        hhead = packer::depackhead(head);
        LOG(INFO) << "start read body";
        do_readbody();
      } else {
        LOG(ERROR) << "read head error: "<< ec.what();
      }
    }
  );
}

void Connection::do_readbody() {
  LOG(INFO) << "start read body";
  async_read(socket, boost::asio::buffer(body, hhead.size),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
        LOG(INFO) << "read body successful";
        switch (hhead.method) {
          case login_success :
            LOG(INFO) << "userinfo write to info";
            Info::info->opinfo([&]{
              Json::Value body_ = packer::depackbody(body, hhead.size);
              Info::info->userinfo.persionalinfo.id = body_["persionalinfo"]["id"].asInt();
              Info::info->userinfo.persionalinfo.password = body_["persionalinfo"]["password"].asInt();
              Info::info->userinfo.persionalinfo.username = body_["persionalinfo"]["username"].asString();
              for(auto x : body_["friendlist"])
                Info::info->userinfo.friendlist.push_back(FriendInfo(x["id"].asInt()));
              for(auto x : body_["messagelist"])
                Info::info->userinfo.messagelist.push_back(
                  MessageInfo(x["sender"].asInt(),
                    x["receiver"].asInt(),
                    x["message"].asString()
                  )
                );
              LOG(INFO) << Info::info->userinfo.persionalinfo.id
                        << Info::info->userinfo.persionalinfo.password
                        << Info::info->userinfo.persionalinfo.username;
              for(auto x : Info::info->userinfo.friendlist)
                LOG(INFO) << x.friendid;
              for(auto x : Info::info->userinfo.messagelist)
                LOG(INFO) << x.sender << x.receiver << x.msg;
            });
            break;
          case signin_success :
            LOG(INFO) << "sign info write to info";
            Info::info->opinfo([&]{
              Json::Value body_ = packer::depackbody(body, hhead.size);
              Info::info->userinfo.persionalinfo.id = body_["persionalinfo"]["id"].asInt();
              Info::info->userinfo.persionalinfo.password = body_["persionalinfo"]["password"].asInt();
              Info::info->userinfo.persionalinfo.username = body_["persionalinfo"]["username"].asString();
            });
            break;
          case chat_success :
            LOG(INFO) << "chat info write to info";
            Info::info->opinfo([&]{
              Json::Value body_ = packer::depackbody(body, hhead.size);
              MessageInfo temp;
              temp.sender = body_["sender"].asInt();
              temp.receiver = body_["receiver"].asInt();
              temp.msg = body_["message"].asString();
              Info::info->userinfo.messagelist.push_back(temp);
            });
            break;
          case addfriend_success :
            LOG(INFO) << "friend info write to info";
            Info::info->opinfo([&]{
              Json::Value body_ = packer::depackbody(body, hhead.size);
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
  std::string message = packer::enpack(status, target);
  LOG(INFO) << "message size:" << message.size();
  char *msg = new char[50009];
  std::copy(message.begin(), message.end(), msg);
  //for(int i = 0; i < message.size(); i ++)
  //  LOG(INFO) << msg[i];
  LOG(INFO) << "do_write message" << '\n';
  async_write(socket, boost::asio::buffer(msg, message.size()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) { LOG(INFO) << "write success" << '\n'; }
  });
}

} // namespace vchat
