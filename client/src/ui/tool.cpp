/*
 * Copyright (c) 2024 Hunter 执着
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "ui.h"
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/color.hpp>
#include <string>

// offered by ftxui author see more for
// https://github.com/ArthurSonzogni/FTXUI/issues/336
Elements split(std::string the_text) {
  Elements output;
  std::stringstream ss(the_text);
  std::string word;
  while (std::getline(ss, word, '\n'))
    output.push_back(text(word));
  return output;
}

Element paragraph_imp(std::string the_text) {
  Elements lines;
  for (Element &line : split(std::move(the_text)))
    lines.push_back(line);
  return vbox(std::move(lines));
}

Component myinput(std::string *ss, const std::string placeholder,
  bool ispassword, const std::string cursor) {
  auto myinput = Renderer([=](bool focused){
    Element show;
    if (focused) {
      if (ispassword) {
        std::string password;
        for(int i = 0 ; i < ss->size(); i ++)
          password += "•";
        show = text(password + cursor);
      }
      else show = text(*ss + cursor);
    } else {
      if (ispassword) {
        std::string password;
        for(int i = 0 ; i < ss->size(); i ++)
          password += "•";
        show = text(password + cursor);
      }
      else show = text(*ss);
      if (!ss->size()) show = text(placeholder);
    }
    return show;
  });
  return CatchEvent(myinput, [=](Event event){
    if (event == Event::a) { *ss += 'a'; return true; }
    else if (event == Event::b) { *ss += 'b'; return true; }
    else if (event == Event::c) { *ss += 'c'; return true; }
    else if (event == Event::d) { *ss += 'd'; return true; }
    else if (event == Event::e) { *ss += 'e'; return true; }
    else if (event == Event::f) { *ss += 'f'; return true; }
    else if (event == Event::g) { *ss += 'g'; return true; }
    else if (event == Event::h) { *ss += 'h'; return true; }
    else if (event == Event::i) { *ss += 'i'; return true; }
    else if (event == Event::j) { *ss += 'j'; return true; }
    else if (event == Event::k) { *ss += 'k'; return true; }
    else if (event == Event::l) { *ss += 'l'; return true; }
    else if (event == Event::m) { *ss += 'm'; return true; }
    else if (event == Event::n) { *ss += 'n'; return true; }
    else if (event == Event::o) { *ss += 'o'; return true; }
    else if (event == Event::p) { *ss += 'p'; return true; }
    else if (event == Event::q) { *ss += 'q'; return true; }
    else if (event == Event::s) { *ss += 's'; return true; }
    else if (event == Event::t) { *ss += 't'; return true; }
    else if (event == Event::u) { *ss += 'u'; return true; }
    else if (event == Event::v) { *ss += 'v'; return true; }
    else if (event == Event::w) { *ss += 'w'; return true; }
    else if (event == Event::x) { *ss += 'x'; return true; }
    else if (event == Event::y) { *ss += 'y'; return true; }
    else if (event == Event::z) { *ss += 'z'; return true; }
    else if (event == Event::A) { *ss += 'A'; return true; }
    else if (event == Event::B) { *ss += 'B'; return true; }
    else if (event == Event::C) { *ss += 'C'; return true; }
    else if (event == Event::D) { *ss += 'D'; return true; }
    else if (event == Event::E) { *ss += 'E'; return true; }
    else if (event == Event::F) { *ss += 'F'; return true; }
    else if (event == Event::G) { *ss += 'G'; return true; }
    else if (event == Event::H) { *ss += 'H'; return true; }
    else if (event == Event::I) { *ss += 'I'; return true; }
    else if (event == Event::J) { *ss += 'J'; return true; }
    else if (event == Event::K) { *ss += 'K'; return true; }
    else if (event == Event::L) { *ss += 'L'; return true; }
    else if (event == Event::M) { *ss += 'M'; return true; }
    else if (event == Event::N) { *ss += 'N'; return true; }
    else if (event == Event::O) { *ss += 'O'; return true; }
    else if (event == Event::P) { *ss += 'P'; return true; }
    else if (event == Event::Q) { *ss += 'Q'; return true; }
    else if (event == Event::S) { *ss += 'S'; return true; }
    else if (event == Event::T) { *ss += 'T'; return true; }
    else if (event == Event::U) { *ss += 'U'; return true; }
    else if (event == Event::V) { *ss += 'V'; return true; }
    else if (event == Event::W) { *ss += 'W'; return true; }
    else if (event == Event::X) { *ss += 'X'; return true; }
    else if (event == Event::Y) { *ss += 'Y'; return true; }
    else if (event == Event::Z) { *ss += 'Z'; return true; }
    else if (event == Event::Character('0')) { *ss += '0'; return true; }
    else if (event == Event::Character('1')) { *ss += '1'; return true; }
    else if (event == Event::Character('2')) { *ss += '2'; return true; }
    else if (event == Event::Character('3')) { *ss += '3'; return true; }
    else if (event == Event::Character('4')) { *ss += '4'; return true; }
    else if (event == Event::Character('5')) { *ss += '5'; return true; }
    else if (event == Event::Character('6')) { *ss += '6'; return true; }
    else if (event == Event::Character('7')) { *ss += '7'; return true; }
    else if (event == Event::Character('8')) { *ss += '8'; return true; }
    else if (event == Event::Character('9')) { *ss += '9'; return true; }
    else if (event == Event::Backspace) { if(ss->size()) ss->pop_back(); return true; }
    return false;
  });
}
