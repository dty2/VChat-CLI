#include "function.h"

namespace vchat {

Function* Function::function = nullptr;

Function::Function() {}

void Function::getinstance() {
  function = new Function();
  control = Connection::getinstance();
  std::thread t_control([&](){
    control->do_connect();
    control->io.run();
  });
  t_control.detach();
}

void Function::do_login() {
}

void Function::do_logout() {
}

void Function::do_signin() {
}

void Function::do_signout() {
}

void Function::do_chat() {
}

void Function::do_addfriend() {
}

void Function::do_deletefriend() {
}

} // namespace vchat
