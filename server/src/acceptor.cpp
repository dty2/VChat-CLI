#include "acceptor.h"

namespace vchat {
using boost::asio::ip::tcp;

Acceptor* accept = nullptr;

Acceptor* Acceptor::getInstance() {
  if(accept == nullptr) accept = new vchat::Acceptor();
  return accept;
}

Acceptor::Acceptor()
  : signals(io),
    acceptor(io, endpoint),
    endpoint(tcp::v4(), port) {
  /// add signals
  signals.add(SIGINT);
  signals.add(SIGTERM);
  signals.add(SIGQUIT);
  connection_manager = ConnectionManager::getInstance(io);
  do_await_stop();
  /// start accept
  do_accept();
}

void vchat::Acceptor::run() { io.run(); }

void vchat::Acceptor::do_accept() {
  acceptor.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        LOG(INFO) << "A Client Connect" << "\n";
        ConnectionManager::connection_manager->start(std::make_shared<Connection>(std::move(socket)));
      }
      /*
       * too much accept can make the stack overflow
       * can't write do_accept directly
       * so need to use defer
      */
      boost::asio::defer(io, [this]{ do_accept(); });
    }
  );
}

} // namespace vchat 
