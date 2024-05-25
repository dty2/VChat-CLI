#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "package.h"
#include "connection.h"

namespace vchat {

class Function {
private:
  std::unordered_set<int> online;
  Function();
  static Connection* control;
public:
  static Function* function;
  static void getinstance();
  Function(const Function&) = delete;
  Function& operator=(const Function&) = delete;

  // request service
  void do_login();
  void do_logout();
  void do_signin();
  void do_signout();
  void do_chat();
  void do_addfriend();
  void do_deletefriend();

  // information operation
  void info_read();
  void info_write();

};

} // namespace vchat

#endif // FUNCTION_H

