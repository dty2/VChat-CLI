#include "acceptor.h"
#include "service.h"

int main() {
  google::InitGoogleLogging("VChat-server");
  google::SetLogDestination(google::GLOG_INFO, "/home/hunter/project/vchat/log/");
  FLAGS_colorlogtostderr = 1;
  FLAGS_alsologtostderr = 1;
  try {
    LOG(INFO) << "start server" << '\n';
    vchat::Service::service = vchat::Service::getInstance();
    vchat::Acceptor::accept = vchat::Acceptor::getInstance();
    vchat::Acceptor::accept->run();
  } catch (const std::exception& e) {
    LOG(ERROR) << "exception: " << e.what() << '\n';
  }
  return 0;
}
