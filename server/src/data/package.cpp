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
