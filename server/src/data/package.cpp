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

#include "package.h"

namespace packer {

std::string enpack(int method) {
  std::string head;
  head += std::to_string(method);
  head += "00000";
  return head;
}

std::string enpack(int method, Json::Value body) {
  std::ostringstream oss;
  Json::StreamWriterBuilder builder;
  std::string output = Json::writeString(builder, body);
  std::string size = std::to_string(output.size());
  oss << std::setfill('0') << std::setw(5) << size;
  std::string head;
  head += std::to_string(method);
  head += oss.str();
  output = head + output;
  return output;
}

std::pair<int, int> depackhead(char *target) {
  int bodysize, headmethod;
  std::string method(target, target + 3);
  headmethod = std::stoi(method);
  std::string size(target + 3, target + 8);
  bodysize = std::stoi(size);
  return std::pair<int, int>(headmethod, bodysize);
}

Json::Value depackbody(char *target, int size) {
  Json::Value root;
  std::string errors;
  Json::CharReaderBuilder builder;
  std::string temp(target, size);
  std::stringstream ss(temp);
  if (Json::parseFromStream(builder, ss, &root, &errors)) {
    return root;
  } else {
    return root;
  }
}

} // namespace packer
