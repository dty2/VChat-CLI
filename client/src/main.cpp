#include "function.h"
#include "connection.h"
#include "info.h"
#include "start.h"

int main(int argc, char **argv) {
  google::InitGoogleLogging("VChat-client");
  google::SetLogDestination(google::GLOG_INFO, "/home/hunter/project/vchat/log/");
  FLAGS_colorlogtostderr = 1;
  FLAGS_alsologtostderr = 1;
  try {
    vchat::Info::getinstance();
    vchat::Connection::getinstance();
    vchat::Function::getinstance();
    std::thread t([&]{
      LOG(INFO) << "IO running..." << '\n';
      vchat::Connection::connection->io.run();
    });
    LOG(INFO) << "tui start..." << '\n';
    vchat::Start start;
    vchat::Chat::getinstance();
    t.join();
  } catch (const std::exception& e) {
    LOG(ERROR) << "exception: " << e.what() << '\n';
  }
  return 0;
}
