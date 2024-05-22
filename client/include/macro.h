#ifndef MACRO_H
#define MACRO_H

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

#endif // MACRO_H
