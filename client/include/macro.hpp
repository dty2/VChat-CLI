// Copyright 2024 Arthur Hunter. All rights reserved.
// Use of this source code is governed by the GPL-3.0 license 
// that can be found in the LICENSE file.

#ifndef MACRO_HPP
#define MACRO_HPP

// dom placeholder
#define dph(a, b) vbox() \
| size(WIDTH, EQUAL, a) \
| size(HEIGHT, EQUAL, b)

// component placeholder
#define cph(a, b) Renderer([](){ \
return vbox() \
| size(WIDTH, EQUAL, a) \
| size(HEIGHT, EQUAL, b); \
})

#endif
