#include "package.h"

namespace vchat {
namespace packer {

std::string getsize(int size_) {
  //TO FIX BUG: when size_ == 30, 30000 will get error
  std::string size;
  size = std::to_string(size_);
  if(size.size() < 5) {
    for(int i = 0; i < size.size() - 5; i ++)
      size += '0';
  }
  LOG(INFO) << "size: " << size << '\n';
  return size;
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
    case signin_success: type = 1; break;
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
  //LOG(INFO) << method;
  head.method = std::stoi(method);
  std::string size(target + 4, target + 9);
  for(int i = 1 ; i < 5; i ++) if(*(target + 9 - i) == '0') size.pop_back(); 
  //LOG(INFO) << size;
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
