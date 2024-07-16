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

#include "server.h"

Server::Server(std::string port_)
  : address("127.0.0.1"), port(port_),
    io_context(1), signals(io_context), acceptor(io_context) {
  signals.add(SIGINT);
  signals.add(SIGTERM);
  signals.add(SIGQUIT);
  boost::asio::ip::tcp::resolver resolver(io_context);
  boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
  acceptor.open(endpoint.protocol());
  acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  acceptor.bind(endpoint);
  acceptor.listen();
  do_await_stop();
  LOG(INFO) << "Do accept on " << port << " port...";
  do_accept();
}

void Server::run() { io_context.run(); }

void Server::do_accept() {
  acceptor.async_accept(
    [&](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        DLOG(INFO) << "A Client connection";
        connection_manager.add(std::make_shared<Connection>(std::move(socket), &connection_manager));
      }
      do_accept();
    }
  );
}

void Server::do_await_stop() {
  signals.async_wait(
    [&](boost::system::error_code ec, int sign) {
      LOG(INFO) << "Start close";
      DLOG(INFO) << "Close acceptor";
      acceptor.close();
      DLOG(INFO) << "Close connection manager";
      connection_manager.stop_all();
      DLOG(INFO) << "Close io_context";
      io_context.stop();
      LOG(INFO) << "Close finish, server exit";
    }
  );
}
