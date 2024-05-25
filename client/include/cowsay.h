#ifndef COWSAY_H
#define COWSAY_H

#include <string>

namespace vchat {

static std::string SPLIT = "│ \n│ \n│ \n│ \n│ \n│ \n│ \n│";

static std::string COWSAY = "\
                      ,__,        \n\
                      (oo)____    \n\
                      (__)    )\\  \n\
                         ||--||   ";

static std::string START = "\
  [ Welcome to VChat... ] \n\
  [ Press j/k to choose option ]";
static std::string QUIT = " [ Quit VChat... ]";
} // namespace vchat

#endif // COWSAY_H
