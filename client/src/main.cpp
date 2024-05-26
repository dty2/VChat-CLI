#include "function.h"
#include "connection.h"
#include "info.h"
#include "start.h"

int main(int argc, char **argv) {
  google::InitGoogleLogging("VChat-client");
  google::SetLogDestination(google::GLOG_INFO, "/home/hunter/project/vchat/log/");
  try {
    vchat::Function::getinstance();
    vchat::Info::getinstance();
    vchat::Start start;
    std::thread t([&](){
      LOG(INFO) << "IO running..." << '\n';
      vchat::Connection::connection->io.run();
    });
    t.detach();
  } catch (const std::exception& e) {
    LOG(ERROR) << "exception: " << e.what() << '\n';
  }
  return 0;
}
