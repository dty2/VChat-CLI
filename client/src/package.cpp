#include "package.h"

namespace vchat {
namespace packer {

int getheadsize() {
  Head head;
  std::ostringstream os;
  boost::archive::text_oarchive oa(os);
  oa << head;
  return os.str().size();
}

std::string enpack(int method, Json::Value body) {
  // enpack head
  Json::StreamWriterBuilder builder;
  std::string output = Json::writeString(builder, body);
  Head head;
  head.type = method >= 100;
  head.size = body.size();
  head.method = method;
  std::ostringstream os;
  boost::archive::text_oarchive oa(os);
  oa << head;
  output = os.str() + output;
  return output;
}
Head depackhead(std::shared_ptr<std::string> target) {
  Head head;
  std::istringstream is(*target);
  boost::archive::text_iarchive ia(is);
  ia >> head;
  return head;
}

Json::Value depackbody(std::shared_ptr<std::string> target) {
  Json::Value root;
  std::string errors;
  Json::CharReaderBuilder builder;
  std::stringstream ss(*target);
  if(Json::parseFromStream(builder, ss, &root, &errors)) {
    return root;
  } else {
    LOG(ERROR) << "parse json error" << '\n';
    return 0;
  }
}

} // namespace packer
} // namespace vchat
