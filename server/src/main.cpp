#include "boost/asio/io_context.hpp"
#include "service.h"
#include "server.h"

void start_glog() {
  FLAGS_alsologtostderr = true;
  FLAGS_log_dir = "/home/hunter/project/vchat/log/";
  FLAGS_max_log_size = 10 * 1024; // 10MB
  FLAGS_minloglevel = google::INFO;
  google::InitGoogleLogging("VChat-Server");
}
void stop_glog() { google::ShutdownGoogleLogging(); }

int main() {
  start_glog();
  try {
    LOG(INFO) << "Start VChat-Server";
    Store::getinstance();
    Service::getinstance();
    Server server;
    server.run();
    Store::free();
  } catch (const std::exception& e) {
    LOG(ERROR) << "exception: " << e.what();
  }
  stop_glog();
  return 0;
}
