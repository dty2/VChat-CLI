#include "net.h"

namespace vchat {

Net::Net(std::function<void(int, Json::Value)> handle_)
  : resolver(io), socket(io), handle(handle_) {
  endpoint = resolver.resolve(address, port);
  head = new char[HEAD_SIZE];
  body = new char[BODY_SIZE];
  boost::asio::async_connect(socket, endpoint, [&](boost::system::error_code ec, tcp::endpoint) {
    if (!ec) {
      LOG(INFO) << "connection successful";
      readhead();
    }
    else {
      LOG(INFO) << "connection error";
    }
  });
}

void Net::start() {
  this->io.run();
}

Net::~Net() { free(head); free(body); }

void Net::close() {
  this->socket.close();
  this->io.stop();
}

void Net::readhead() {
  LOG(INFO) << "start read head";
  async_read(socket, boost::asio::buffer(head, HEAD_SIZE),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
    LOG(INFO) << "read head successful";
    if (!ec) {
      LOG(INFO) << "read head successful";
      readbody(packer::depackhead(head));
    } else {
      this->close();
    }
  });
  LOG(INFO) << " read head end";
}

void Net::readbody(std::pair<int, int> headinfo) {
  LOG(INFO) << "start read body";
  async_read(socket, boost::asio::buffer(body, headinfo.second),
  [=](boost::system::error_code ec, std::size_t bytes_transferred) {
    if(!ec) {
      LOG(INFO) << "this is headinfo" << headinfo.first << ' ' << headinfo.second;
      handle(headinfo.first, packer::depackbody(body, headinfo.second));
      readhead();
    } else {
      DLOG(ERROR) << "read head error: "<< ec.what();
    }
  });
}

bool Net::write(int status) {
  std::string message = packer::enpack(status);
  char *msg = new char[ALL_SIZE];
  std::copy(message.begin(), message.end(), msg);
  DLOG(INFO) << "write message";
  boost::system::error_code ec;
  boost::asio::write(socket, boost::asio::buffer(msg, message.size()), ec);
  if(ec) {
    DLOG(ERROR) << "write error: "<< ec.what();
    return 1; // 发送数据失败
  }
  return 0;
}

bool Net::write(int status, Json::Value& target) {
  std::string message = packer::enpack(status, target);
  if (message.size() > ALL_SIZE) { return 2; } // 传输信息太大
  char *msg = new char[ALL_SIZE];
  std::copy(message.begin(), message.end(), msg);
  DLOG(INFO) << "write message" << '\n';
  boost::system::error_code ec;
  boost::asio::write(socket, boost::asio::buffer(msg, message.size()), ec);
  delete[] msg;
  if(ec) {
    DLOG(ERROR) << "write error: "<< ec.what();
    return 1; // 发送数据失败
  }
  return 0;
}

} // namespace vchat
