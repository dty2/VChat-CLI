#include "package.h"

namespace vchat {

template<typename Type>
int Packer::getbodysize(Type target) {
  return log_in;
}

template<typename Type>
std::string Packer::enpack(Type target, bool type) {
  struct Package<Type> temp;
  std::ostringstream os;
  boost::archive::text_oarchive oa(os);
  temp.head.type = type;
  temp.head.size = getbodysize(target);
  temp.head.operate = getbodysize(target);
  temp.body = target;
  oa << temp;
  return os.str();
}

template<typename Type>
Type Packer::depack(std::array<char, body_size>& target) {
  Type temp;
  std::string tran;
  for(int i = 0; i < body_size; i ++) tran.push_back(target[i]);
  std::istringstream is(tran);
  boost::archive::text_iarchive ia(is);
  ia >> temp;
  return temp;
}

Head Packer::depack(std::array<char, head_size>& target) {
  Head temp;
  std::string tran;
  for(int i = 0; i < head_size; i ++) tran.push_back(target[i]);
  std::istringstream is(tran);
  boost::archive::text_iarchive ia(is);
  ia >> temp;
  return temp;
}

} // namespace packages
