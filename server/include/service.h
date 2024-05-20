#ifndef WORK_H
#define WORK_H

#include "common.h"
#include "macro.h"

extern std::unordered_map<std::string, UserInfo> userlist;

class Work {
private:
  static std::unordered_map<std::string, std::shared_ptr<Chat_server>> chat_server_list;
  boost::asio::io_context& io_context;
  std::shared_ptr<tcp::socket> socket;

  void receive_msg();
  void send_msg(Message);
  void find_receiver(Message);
  
public:
  Chat_server(boost::asio::io_context&, std::shared_ptr<tcp::socket>);
  static void create(std::string, boost::asio::io_context&, std::shared_ptr<tcp::socket>);
  void get_info(std::shared_ptr<tcp::socket>); // 读取登录信息
  bool log_judge(std::shared_ptr<LogInfo>);
  void return_permission(std::shared_ptr<tcp::socket>, std::shared_ptr<LogInfo>); // 返回登录许可
  void return_userinfo(std::shared_ptr<tcp::socket>, std::string); // 登录许可为1，返回用户信息

};
#endif // BUSINESS_H
