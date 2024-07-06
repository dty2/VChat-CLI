#include "net.h"

namespace vchat {

Net::Net() : resolver(io), socket(io) {
  endpoint = resolver.resolve(address, port);
  head = new char[HEAD_SIZE];
  body = new char[BODY_SIZE];
}

Net::~Net() { free(head); free(body); }

bool Net::connect() {
  boost::system::error_code ec;
  boost::asio::connect(socket, endpoint, ec);
  if(ec) {
    DLOG(ERROR) << "connect error: "<< ec.what();
    return 1; // 连接服务器失败
  }
  return 0;
}

bool Net::readhead(std::pair<int, int>& info) {
  DLOG(INFO) << "start read head";
  boost::system::error_code ec;
  boost::asio::read(socket, boost::asio::buffer(head, HEAD_SIZE));
  if(!ec) {
    DLOG(INFO) << "read head successful";
    info = packer::depackhead(head);
  } else {
    DLOG(ERROR) << "read head error: "<< ec.what();
    return 1; // 读包头失败
  }
  return 0;
}

bool Net::readbody(int bodysize, Json::Value& target) {
  DLOG(INFO) << "start read body";
  boost::system::error_code ec;
  boost::asio::read(socket, boost::asio::buffer(body, bodysize));
  if(!ec) {
    DLOG(INFO) << "read body successful";
    target = packer::depackbody(body, bodysize);
  } else {
    DLOG(ERROR) << "read head error: "<< ec.what();
    return 1; // 读包头失败
  }
  return 0;
}

bool Net::read(int& status, Json::Value& target) {
  std::pair<int, int> info;
  if (readhead(info)) { return 1; } // 读包头失败
  status = info.first;
  if (readbody(info.second, target)) { return 2; } // 读包身失败
  return 0;
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
