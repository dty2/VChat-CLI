#include "server.h"

using boost::asio::ip::tcp;

Server::Server() : signals(io_context), acceptor(io_context), endpoint(tcp::v4(), port) {
  signals.add(SIGINT);
  signals.add(SIGTERM);
  do_await_stop();
  do_accept();
  LOG(INFO) << "Start Accept...";
  do_accept();
}

void Server::run() { io_context.run(); }

void Server::do_accept() {
  acceptor.async_accept(
    [&](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        connection_manager.add(std::make_shared<Connection>(std::move(socket), &connection_manager));
      }
      do_accept();
    }
  );
}

void Server::do_await_stop() {
  signals.async_wait(
    [&](boost::system::error_code ec, int sign) {
      LOG(INFO) << "Server start close...";
      acceptor.close();
      LOG(INFO) << "Connection manager start stop...";
      connection_manager.stop_all();
      LOG(INFO) << "Server quit...";
    }
  );
}
