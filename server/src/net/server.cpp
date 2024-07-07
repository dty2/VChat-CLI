#include "server.h"

Server::Server() : io_context(1), signals(io_context), acceptor(io_context) {
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
  LOG(INFO) << "Start Accept...";
  do_accept();
}

void Server::run() { io_context.run(); }

void Server::do_accept() {
  acceptor.async_accept(
    [&](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        LOG(INFO) << "new client connection";
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
