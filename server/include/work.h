#ifndef WORK_H
#define WORK_H

#include "service.h"

namespace vchat {

class WorkManager {
private:
  WorkManager();
public:
  WorkManager(const WorkManager&) = delete;
  WorkManager& operator=(const WorkManager&) = delete;
  static WorkManager* getInstance();
  static void push_work(Head, Json::Value, std::function<void(int, Json::Value)>);
  void do_work(std::function<void(Json::Value, std::function<void(int, Json::Value)>)>);
};

} // namespace vchat

#endif // WORK_H
