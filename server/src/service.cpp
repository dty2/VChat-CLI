#include "work.h"

std::unordered_map<std::string, std::shared_ptr<Work>> Work::Work_list;

Work::Work(boost::asio::io_context& io_context_, std::shared_ptr<tcp::socket> socket_)
  : io_context(io_context_), socket(socket_)
{
  receive_msg();
}

void Work::create(std::string account, boost::asio::io_context& io_context, std::shared_ptr<tcp::socket> socket)
{
  std::shared_ptr<Work> chatserver = std::make_shared<Work>(io_context, socket);
  Work_list[account] = chatserver;
}

void Work::receive_msg()
{
  std::shared_ptr<boost::asio::streambuf> sb = std::make_shared<boost::asio::streambuf>();
  boost::asio::async_read_until(*socket, *sb, '#',
      [this, sb](const boost::system::error_code ec, std::size_t)
      {
          std::istream is(sb.get());
          boost::archive::text_iarchive ia(is);
          Message msg;
          ia >> msg;
          sb->consume(sb->size()); // 清空streambuf
          LOG(INFO) << "server receive the msg from " << msg.sender << "\n";
          find_receiver(std::move(msg));
          boost::asio::defer(io_context, [this](){
              receive_msg();
          });
      }
  );
}

void Work::send_msg(Message msg)
{
  std::ostringstream os;
  boost::archive::text_oarchive oa(os);
  oa << msg;
  std::shared_ptr<std::string> send_userinfo = std::make_shared<std::string>(os.str() + '#');
  boost::asio::async_write(*socket,
      boost::asio::buffer(*send_userinfo, send_userinfo->size()),
      boost::asio::transfer_all(),
      [msg](const boost::system::error_code ec, std::size_t)
      {
          LOG(INFO) << "server send message to " << msg.receiver << "\n";
      }
  );
}

void Work::find_receiver(Message msg)
{
  for(auto& [account, item] : Work_list)
  {
      if(msg.receiver == account)
      {
          item->send_msg(msg);
          LOG(INFO) << "server find receiver " << msg.receiver << "\n";
      }
  }
}

// 接收用户登录信息
void vchat::Transmission::get_info(std::shared_ptr<tcp::socket> socket)
{
    /*
        传输问题1:
        这里为什么不直接使用binary_(i/o)archive呢？
        因为binary_(i/o)archive会是二进制流，会因平台不同出问题(服务端(linux)和客户端(windows)平台不一致)

        传输问题2:
        这里为什么要使用read_until呢？
        因为每次传输的大小不是固定的

        传输问题3:
        发送端发送的是string，为什么接收端不能同样用string接受呢？
        因为发送端虽然发的是string，但是其大小是固定的了，我们在发送的时候已经指定了其发送数据的大小了
        而接收端在接受的时候，虽然我们使用的是string，但是我们也要指定大小，但是每次传输的大小不是一致的

        传输问题4:
        为什么我的发送端发出消息，接收端收到了，但是不会触发回调函数呢？
        情况一: 见客户端注释
        情况二: 因为指定的大小缘故。只有读到MAX_size,async_read()才会返回
        即，当接收端指定的接受大小大于发送端的时候，其读取函数是不会返回的，会阻塞住
    */
    std::shared_ptr<boost::asio::streambuf> sb = std::make_shared<boost::asio::streambuf>();
    std::shared_ptr<LogInfo> loginfo = std::make_shared<LogInfo>();
    boost::asio::async_read_until(*socket, *sb, '#',
      [this, socket, loginfo, sb](const boost::system::error_code ec, std::size_t)
      {
        /*
          * a joke: don't be anger when you talking ...
          * maybe the opposite is a "streambuf(sb)"
        */
        std::istream is(sb.get()); // *sb 会报错，不知道为啥
        boost::archive::text_iarchive ia(is);
        ia >> *loginfo;
        sb->consume(sb->size()); // 清空streambuf
        LOG(INFO) << "callback: the connect account is " << loginfo->account << "\n";
        LOG(INFO) << "callback: the connect password is " << loginfo->password << "\n";
        return_permission(socket, loginfo);
      }
    );
}

void vchat::Transmission::return_permission(std::shared_ptr<tcp::socket> socket, std::shared_ptr<LogInfo> loginfo)
{
    LOG(INFO) << "start to send permission to client!" << "\n";
    boost::asio::async_write(*socket,
        boost::asio::buffer((log_judge(loginfo) ? "1" : "0"), 1),
        boost::asio::transfer_all(),
        [this, socket, loginfo](const boost::system::error_code ec, std::size_t)
        {
            LOG(INFO) << "send permission " << log_judge(loginfo) << "\n";
            if(log_judge(loginfo)) return_userinfo(socket, loginfo->account);
        }
    );
}

void vchat::Transmission::return_userinfo(std::shared_ptr<tcp::socket> socket, std::string account_)
{
    LOG(INFO) << "start to send userinfo to client!" << "\n";
    UserInfo userinfo;
    for(auto& [account, item] : userlist)
      if(account == account_)
        userinfo = item;
    std::shared_ptr<std::string> account = std::make_shared<std::string>(userinfo.loginfo.account);
    std::ostringstream os;
    boost::archive::text_oarchive oa(os);
    oa << userinfo;
    std::shared_ptr<std::string> send_userinfo = std::make_shared<std::string>(os.str() + '#');
    boost::asio::async_write(*socket,
      boost::asio::buffer(*send_userinfo, send_userinfo->size()),
      boost::asio::transfer_all(),
      [this, socket, account](const boost::system::error_code ec, std::size_t)
      {
        LOG(INFO) << "send userinfo to client successful !" << "\n";
        Chat_server::create(*account, io_context, socket);
      }
    );
}

} // namespace vchat
