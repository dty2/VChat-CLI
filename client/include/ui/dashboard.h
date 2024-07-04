#ifndef DASHBOARD_H
#define DASHBOARD_H

#include "ui.h"

namespace vchat {

using namespace ftxui;

enum { none = 0, log, sign };

class Dashboard {
private:
  int dialog = none;
  int choice = 0;
  InputOption option;
  int log_selected = 0;
  int sign_selected = 0;
  std::string id, username, password;
  std::function<void(int)> pfun;

  // dialog
  Component dialog_log();
  int log_but = 0;
  Component dialog_sign();
  int sign_but = 0;
public:
  Component content;
  Dashboard(std::function<void(int)>);
};

} // namespace vchat

#endif // DASHBOARD_H
