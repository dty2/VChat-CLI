#include "acceptor.h"
#include "service.h"

int main() {
  FLAGS_alsologtostderr = true;
  FLAGS_log_dir = "/home/hunter/project/vchat/log/";
  FLAGS_max_log_size = 10 * 1024; // 10MB
  FLAGS_minloglevel = google::INFO;
  google::InitGoogleLogging("VChat-Server");
  try {
    LOG(INFO) << "start server" << '\n';
    vchat::Service::service = vchat::Service::getInstance();
    vchat::Acceptor::accept = vchat::Acceptor::getInstance();
    std::thread t([&]{vchat::Acceptor::accept->run();});
    t.join();
  } catch (const std::exception& e) {
    LOG(ERROR) << "exception: " << e.what() << '\n';
  }
  return 0;
}
