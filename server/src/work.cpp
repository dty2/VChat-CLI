#include "work.h"

namespace vchat {

WorkManager::WorkManager() {}

WorkManager* WorkManager::getInstance() {
  WorkManager* instance = new WorkManager();
  return instance;
}

void WorkManager::push_work(
  Head head, Json::Value value,
  std::function<void(int, Json::Value)> callback) {
  if(!head.type) {
    switch (head.method) {
      case login        : Service::service->do_login(value, callback);        break;
      case logout       : Service::service->do_logout(value, callback);       break;
      case signin       : Service::service->do_signin(value, callback);       break;
      case signout      : Service::service->do_signout(value, callback);      break;
      case chat         : Service::service->do_chat(value, callback);         break;
      case addfriend    : Service::service->do_addfriend(value, callback);    break;
      case deletefriend : Service::service->do_deletefriend(value, callback); break;
      default: LOG(ERROR) << "no request service error" << '\n';
    }
  } else {
    switch (head.method) {
      case chat_success      : LOG(INFO)  << "request success" << '\n'; break;
      case chat_error        : LOG(ERROR) << "request error"   << '\n'; break;
      default: LOG(ERROR) << "no response service error" << '\n';
    }
  }
}

} // namespace vchat
