#include "function.h"

namespace vchat {

Function* Function::function = nullptr;
Connection* Function::control = nullptr;
std::thread* Function::t = nullptr;

Function::Function() {}

void Function::getinstance() {
  function = new Function();
  control = Connection::getinstance();
  t = new std::thread([&](){
    control->do_connect();
    control->io.run();
  });
  t->detach();
}

bool Function::do_login(int id, int password) {

  return 0;
}

bool Function::do_logout(int id) {
  return 0;
}

bool Function::do_signin(int id, int password, std::string username) {
  return 0;
}

bool Function::do_signout(int id) {
  return 0;
}

bool Function::do_chat(int sender, int receiver, std::string message) {
  return 0;
}

bool Function::do_addfriend(int id) {
  return 0;
}

bool Function::do_deletefriend(int id) {
return 0;  
}

} // namespace vchat
