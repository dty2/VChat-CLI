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

#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include "common.h"

using boost::asio::ip::tcp;

class Service;
class Connection;
typedef std::shared_ptr<Connection> Connection_ptr;

class ConnectionManager {
public:
  ConnectionManager();
  void add(Connection_ptr);
  void stop(Connection_ptr);
  void stop_all();
  std::set<Connection_ptr> connections;
  friend Connection;
};

class Connection : public std::enable_shared_from_this<Connection> {
public:
  Connection(tcp::socket, ConnectionManager*);
  ~Connection();
  void start();
  void stop();
  void write(int, Json::Value);

private:
  ConnectionManager* manager;
  tcp::socket socket;
  char *head, *body;
  std::pair<int, int> headinfo;
  void readhead();
  void readbody();
};

#endif //CONNECTION_H

