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

#include "connection.h"
#include "package.h"
#include "server.h"
#include "service.h"

using boost::asio::ip::tcp;

ConnectionManager::ConnectionManager() {}

void ConnectionManager::add(Connection_ptr cp) {
  DLOG(INFO) << "Connection manager add";
  connections.insert(cp);
  cp->start();
}

void ConnectionManager::stop(Connection_ptr cp) {
  connections.erase(cp);
  cp->stop();
}

void ConnectionManager::stop_all() {
  for(auto& v : connections)
    v->stop();
  connections.clear();
}

Connection::Connection(tcp::socket socket_, ConnectionManager* manager_)
  : socket(std::move(socket_)), manager(manager_) {
  head = new char[HEAD_SIZE];
  body = new char[BODY_SIZE];
}

Connection::~Connection() {
  free(head);
  free(body);
}

void Connection::start() {
  DLOG(INFO) << "Connection start";
  readhead();
}

void Connection::stop() { socket.close(); }

void Connection::readhead() {
  async_read(socket, boost::asio::buffer(head, HEAD_SIZE),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        DLOG(INFO) << "connetion readhead successful";
        headinfo = packer::depackhead(head);
        readbody();
      } else {
        this->stop();
        manager->connections.erase(shared_from_this());
      }
    }
  );
}

void Connection::readbody() {
  async_read(socket, boost::asio::buffer(body, headinfo.second),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if(!ec) {
        DLOG(INFO) << "connetion readbody successful";
        Json::Value value = packer::depackbody(body, headinfo.second);
        Service::serve(headinfo.first, std::move(value), this);
      }
      readhead();
    }
  );
}

void Connection::write(int status, Json::Value target) {
  std::string message = packer::enpack(status, target);
  char *msg = new char[ALL_SIZE];
  std::copy(message.begin(), message.end(), msg);
  async_write(socket, boost::asio::buffer(msg, message.size()),
    [&](boost::system::error_code ec, std::size_t bytes_transferred) {
      if (!ec) {
        DLOG(INFO) << "Connetion write successful";
      } else {
        this->stop();
        manager->connections.erase(shared_from_this());
      }
  });
}
