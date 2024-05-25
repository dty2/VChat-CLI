#include <ftxui/dom/elements.hpp>
#include <sstream>

namespace vchat {

using namespace ftxui;
// offered by ftxui author see more for https://github.com/ArthurSonzogni/FTXUI/issues/336
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
  for(Element &line : split(std::move(the_text)))
    lines.push_back(line);
  return vbox(std::move(lines));
}

} // namespace vchat
