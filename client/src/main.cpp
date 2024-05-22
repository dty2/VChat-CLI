#include "connection.h"
#include "tui.h"

int main(int argc, char **argv) {
  vchat::Connection* control = vchat::Connection::getinstance();
  vchat::Tui* tui = vchat::Tui::getinstance();
  std::thread t_control([&](){
    control->do_connect();
    control->io.run();
  });
  t_control.join();
  return 0;
}
