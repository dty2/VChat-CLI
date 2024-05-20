// Copyright 2024 Arthur Hunter. All rights reserved.
// Use of this source code is governed by the GPL-3.0 license 
// that can be found in the LICENSE file.

#ifndef HELP_HPP
#define HELP_HPP

#include "common.hpp"

namespace tui {

static std::string HELP = "\
---------------------使用说明书---------------------- \n\
                                                      \n\
                      操作方式                        \n\
                两种模式: chat / command              \n\
                      chat模式                        \n\
            |  keycode  |     behavior    |           \n\
            |-----------|-----------------|           \n\
            |    esc    | 进入command模式 |           \n\
            |   enter   |     发送消息    |           \n\
            |                                         \n\
            |       command模式                       \n\
            |  keycode  |     behavior    |           \n\
            |-----------|-----------------|           \n\
            |   esc     |   进入chat模式  |           \n\
            |    k      |       上        |           \n\
            |    j      |       下        |           \n\
            |    h      |       左        |           \n\
            |    l      |       右        |           \n\
            |  enter    |      确认       |           \n\
            |    q      |      关闭       |           \n";

}

#endif
