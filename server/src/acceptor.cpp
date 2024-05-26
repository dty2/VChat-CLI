#include "acceptor.h"

namespace vchat {
using boost::asio::ip::tcp;

Acceptor* Acceptor::accept = nullptr;

Acceptor* Acceptor::getInstance() {
  if(accept == nullptr) accept = new vchat::Acceptor();
  return accept;
}

Acceptor::Acceptor() : signals(io), acceptor(io, endpoint), endpoint(tcp::v4(), port) {
  /// add signals
  signals.add(SIGINT);
  signals.add(SIGTERM);
  signals.add(SIGQUIT);
  connection_manager = ConnectionManager::getInstance(io);
  do_await_stop();
  LOG(INFO) << "start accept..." << '\n';
  do_accept();
}

void Acceptor::run() { io.run(); }

void Acceptor::do_accept() {
  acceptor.async_accept(
    [&](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        LOG(INFO) << "A Client Connect" << "\n";
        connection_manager->start(std::make_shared<Connection>(std::move(socket), connection_manager));
      }
      /*
       * too much accept can make the stack overflow
       * can't write do_accept directly
       * so need to use defer
      */
      boost::asio::defer(io, [&]{ do_accept(); });
    }
  );
}

void Acceptor::do_await_stop() {
  signals.async_wait(
    [&](boost::system::error_code ec, int sign) {
      LOG(INFO) << "acceptor start close..." << '\n';
      acceptor.close();
      LOG(INFO) << "connection_manager start stop..." << '\n';
      connection_manager->stop_all();
      LOG(INFO) << "server quit..." << '\n';
      exit(0);
    }
  );
}

} // namespace vchat 
