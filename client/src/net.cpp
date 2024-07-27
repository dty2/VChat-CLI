/*
 * Copyright (c) 2024 Hunter 执着
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "net.h"

Net::Net(std::string address_, std::string port_,
  std::function<void(int, Json::Value)> handle_)
  : address(address_), port(port_),
    resolver(io), socket(io), handle(handle_) {
  endpoint = resolver.resolve(address, port);
  head = new char[HEAD_SIZE];
  body = new char[BODY_SIZE];
  LOG(INFO) << "Start connect server " << address << ":" << port;
  boost::asio::async_connect(socket, endpoint,
    [&](boost::system::error_code ec, tcp::endpoint) {
      if (!ec) {
        LOG(INFO) << "Connect server " << address << ":" << port << " successful";
        readhead();
      }
    }
  );
}

void Net::start() {
  LOG(INFO) << "net start";
  LOG(INFO) << "io_context run";
  this->io.run();
}

Net::~Net() { free(head); free(body); }

void Net::close() {
  this->io.stop();
}

void Net::readhead() {
  async_read(socket, boost::asio::buffer(head, HEAD_SIZE),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        DLOG(INFO) << "readhead successful";
        readbody(packer::depackhead(head));
      } else {
        this->close();
        LOG(INFO) << "readhead error";
      }
  });
}

void Net::readbody(std::pair<int, int> headinfo) {
  async_read(socket, boost::asio::buffer(body, headinfo.second),
  [=](boost::system::error_code ec, std::size_t bytes_transferred) {
    if(!ec) {
      DLOG(INFO) << "readbody successful";
      handle(headinfo.first, packer::depackbody(body, headinfo.second));
      readhead();
    } else {
      LOG(INFO) << "readbody error";
    }
  });
}

bool Net::write(int status) {
  std::string message = packer::enpack(status);
  char *msg = new char[ALL_SIZE];
  std::copy(message.begin(), message.end(), msg);
  boost::system::error_code ec;
  boost::asio::write(socket, boost::asio::buffer(msg, message.size()), ec);
  if(ec) {
    return 1; // 发送数据失败
  }
  return 0;
}

bool Net::write(int status, Json::Value& target) {
  LOG(INFO) << "write info";
  std::string message = packer::enpack(status, target);
  if (message.size() > ALL_SIZE) { return 2; } // 传输信息太大
  char msg[ALL_SIZE];
  std::copy(message.begin(), message.end(), msg);
  boost::system::error_code ec;
  boost::asio::write(socket, boost::asio::buffer(msg, message.size()), ec);
  if(ec) {
    DLOG(INFO) << "write info error";
    return 1; // 发送数据失败
  }
  return 0;
}
