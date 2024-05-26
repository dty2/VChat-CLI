#ifndef FUNCTION_H
#define FUNCTION_H

#include "common.h"
#include "package.h"
#include "connection.h"

namespace vchat {

class Connection;

class Function {
public:
  static Function* function;
  static Connection* connect;
  static void getinstance();
  Function(const Function&) = delete;
  Function& operator=(const Function&) = delete;

  // request service
  void do_login(int, int);
  void do_logout(int);
  void do_signin(int, int, std::string);
  void do_signout(int);
  void do_chat(int, int, std::string);
  void do_addfriend(int);
  void do_deletefriend(int);

private:
  Function();
  ~Function();

};

} // namespace vchat

#endif // FUNCTION_H

