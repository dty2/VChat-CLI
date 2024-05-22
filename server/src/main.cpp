#include "acceptor.h"
#include "service.h"

int main() {
  try {
    vchat::Service::service = vchat::Service::getInstance();
    vchat::Acceptor::accept = vchat::Acceptor::getInstance();
    vchat::Acceptor::accept->run();
  } catch (const std::exception& e) {
    LOG(ERROR) << "exception: " << e.what() << "\n";
  }
  return 0;
}
