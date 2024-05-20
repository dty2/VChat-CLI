// Copyright 2024 Arthur Hunter. All rights reserved.
// Use of this source code is governed by the GPL-3.0 license 
// that can be found in the LICENSE file.

#ifndef CONTROL_HPP
#define CONTROL_HPP 

#include "common.hpp"
#include "tui.hpp"
#include "transmission.hpp"

class Control {
private:
  static std::unique_ptr<Control> control;
  Control();
  Control(const Control& t) = delete;
  Control& operator=(const Control& t) = delete;

  // tui
  std::unique_ptr<tui::Module> tui;

  //tran
  std::unique_ptr<tran::Module> tran;

public:
  static void getinstance();
};

#endif
