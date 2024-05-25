#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "package.h"
#include "connection.h"

namespace vchat {

class Function {
private:
  static Connection* control;
  static std::thread* t;

  Function();

public:
  static Function* function;
  static void getinstance();
  Function(const Function&) = delete;
  Function& operator=(const Function&) = delete;

  // request service
  bool do_login(int, int);
  bool do_logout(int);
  bool do_signin(int, int, std::string);
  bool do_signout(int);
  bool do_chat(int, int, std::string);
  bool do_addfriend(int);
  bool do_deletefriend(int);

  // information operation
  void info_read();
  void info_write();

};

} // namespace vchat

#endif // FUNCTION_H

