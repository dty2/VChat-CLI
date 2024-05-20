// Copyright 2024 Arthur Hunter. All rights reserved.
// Use of this source code is governed by the GPL-3.0 license 
// that can be found in the LICENSE file.

#include "control.hpp"

std::unique_ptr<Control> Control::control= nullptr;

void Control::getinstance()
{
  control = std::unique_ptr<Control>(new Control());
}

Control::Control()
{
  // init
  this->tui = std::make_unique<tui::Module>();
  this->tran = std::make_unique<tran::Module>();

  // start
  tui->start();
}
