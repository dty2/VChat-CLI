#include "package.h"

namespace vchat {
namespace packer {

std::string enpack(int method) {
  std::string head;
  head += std::to_string(method);
  head += "00000";
  DLOG(INFO) << "output: " << head << "output size: " << head.size();
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
  DLOG(INFO) << "output: " << output << "output size: " << output.size();
  return output;
}

std::pair<int, int> depackhead(char *target) {
  int headsize, headmethod;
  std::string method(target, target + 3);
  headmethod = std::stoi(method);
  std::string size(target + 3, target + 8);
  headsize = std::stoi(size);
  DLOG(INFO) << "head size:" << headsize << "head method:" << headmethod;
  return std::pair<int, int>(headmethod, headsize);
}

Json::Value depackbody(char *target, int size) {
  DLOG(INFO) << "start depack body";
  Json::Value root;
  std::string errors;
  Json::CharReaderBuilder builder;
  std::string temp(target, size);
  std::stringstream ss(temp);
  if (Json::parseFromStream(builder, ss, &root, &errors)) {
    LOG(INFO) << "depack body success";
    return root;
  } else {
    LOG(ERROR) << "parse json error";
    return root;
  }
}

} // namespace packer
} // namespace vchat
