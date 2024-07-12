#include "tui.h"
#include "function.h"
#include "info.h"

void start_glog() {
  FLAGS_alsologtostderr = true;
  FLAGS_log_dir = "/home/hunter/source/project/vchat/log/";
  FLAGS_max_log_size = 10 * 1024; // 10MB
  FLAGS_minloglevel = google::INFO;
  google::InitGoogleLogging("VChat-Client");
}

void stop_glog() { google::ShutdownGoogleLogging(); }

using namespace vchat;

int main(int argc, char **argv) {
  start_glog();
  try {
    Info::getinstance();
    Tui ui;
  } catch (const std::exception &e) {
    LOG(ERROR) << "Exception: " << e.what() << '\n';
  }
  stop_glog();
  return 0;
}
