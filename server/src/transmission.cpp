#include "transmission.h"

namespace vchat {
using boost::asio::ip::tcp;

Transmission* transmission = nullptr;

Transmission* Transmission::getInstance() {
  if(transmission == nullptr) transmission = new vchat::Transmission();
  return transmission;
}

Transmission::Transmission()
  : signals(io),
    acceptor(io, endpoint),
    connection_manager(io),
    endpoint(tcp::v4(), port) {
  /// add signals
  signals.add(SIGINT);
  signals.add(SIGTERM);
  signals.add(SIGQUIT);

  do_await_stop();
  /// start accept
  do_accept();
}

void vchat::Transmission::run() { io.run(); }

void vchat::Transmission::do_accept() {
  acceptor.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket) {
      if (!ec) {
        LOG(INFO) << "A Client Connect" << "\n";
        connection_manager.start(std::make_shared<Connection>(std::move(socket)));
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
