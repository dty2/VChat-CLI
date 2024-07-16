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

#ifndef NET_H
#define NET_H

#include "common.h"
#include "package.h"

#define HEAD_SIZE 8
#define BODY_SIZE 50000
#define ALL_SIZE 50008

using boost::asio::ip::tcp;

class Net {
private:
  std::string address, port;
  std::function<void(int, Json::Value)> handle;
  boost::asio::io_context io;
  tcp::socket socket;
  tcp::resolver resolver;
  tcp::resolver::results_type endpoint;
  char *head, *body;
  void readhead();
  void readbody(std::pair<int, int>);

public:
  Net(std::string, std::string, std::function<void(int, Json::Value)>);
  ~Net();
  void start();
  void close();
  bool write(int);
  bool write(int, Json::Value&);

};

#endif // NET_H
