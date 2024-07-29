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

static bool handleCharacterEvent(Event event, char character, std::string* ss) {
  if (event == Event::Character(character)) {
    *ss += character;
    return true;
  }
  return false;
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
        show = text(password);
      }
      else show = text(*ss);
      if (!ss->size()) show = text(placeholder);
    }
    return show;
  });
  return CatchEvent(myinput, [=](Event event){
    for (char c = 'a'; c <= 'z'; c ++) {
      if (handleCharacterEvent(event, c, ss)) return true;
      if (handleCharacterEvent(event, toupper(c), ss)) return true;
    }
    for (char c = '0'; c <= '9'; c ++)
      if (handleCharacterEvent(event, c, ss)) return true;
    if (event == Event::Backspace) { if(ss->size()) ss->pop_back(); return true; }
    return false;
  });
}
