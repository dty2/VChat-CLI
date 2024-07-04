#include "package.h"

namespace vchat {
namespace packer {

std::string getsize(int size_) {
  std::string size;
  size = std::to_string(size_);
  std::ostringstream oss;
  oss << std::setfill('0') << std::setw(5) << size;
  return oss.str();
}

std::string enpack(int method, Json::Value body) {
  // enpack std::string
  Json::StreamWriterBuilder builder;
  std::string output = Json::writeString(builder, body);
  std::string head;
  int type;
  switch (method) {
    case login: type = 0; break;
    case signin: type = 1; break;
    case chat: type = 0; break;
    case addfriend: type = 1; break;
    case login_success: type = 0; break;
    case chat_success: type = 0; break;
    case addfriend_success: type = 1; break;
  }
  head += std::to_string(type);
  head += std::to_string(method);
  head += getsize(output.size());
  output = head + output;
  return output;
}

Head depackhead(char* target) {
  Head head;
  head.type = *target - '0';
  std::string method(target + 1, target + 4);
  head.method = std::stoi(method);
  std::string size_0(target + 4, target + 9);
  std::string size;
  int i = 0;
  for(; i < size_0.size(); i ++)
    if(size[i] != '0') break;
  for(; i < size_0.size(); i ++)
    size += size_0[i];
  head.size = std::stoi(size);
  LOG(INFO) << "depackstd::string successful" << '\n';
  return head;
}

Json::Value depackbody(char* target, int size) {
  Json::Value root;
  std::string errors;
  Json::CharReaderBuilder builder;
  std::string temp(target, size);
  std::stringstream ss(temp);
  if(Json::parseFromStream(builder, ss, &root, &errors)) {
    return root;
  } else {
    LOG(ERROR) << "parse json error" << '\n';
    return 0;
  }
}

} // namespace packer
} // namespace vchat
