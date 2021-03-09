#include <deque>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <cstdlib>
#include <boost/asio.hpp>
#include "chatMessage.h"

using boost::asio::ip::tcp;

//----------------------------------------------------------------

using chat_message_queue = std::deque<chat_message>;

//----------------------------------------------------------------

class chat_participant
{
public:
  virtual ~chat_participant() {}
  virtual void deliver(const chat_message &msg) = 0;
};

using chat_participant_ptr = std::shared_ptr<chat_participant>;

//----------------------------------------------------------------

class chat_room
{
public:
  chat_room(boost::asio::io_context &io_context)
    : strand_(io_context)
  {
    
  }
  
  void join(chat_participant_ptr participant)
  {
    strand_.post([this, participant]()
    {
      participants_.insert(participant);
      for (const auto &msg : recent_msgs_)
	participant->deliver(msg);
    });
  }

  void leave(chat_participant_ptr participant)
  {
    strand_.post([this, participant]()
    {
      participants_.erase(participant);
    });
  }

  void deliver(const chat_message &msg)
  {
    strand_.post([this, &msg]()
    {
      recent_msgs_.push_back(msg);
      while (recent_msgs_.size() > max_recent_msgs)
	recent_msgs_.pop_front();

      for (const auto &participant : participants_)
	participant->deliver(msg);  
    });
  }

private:
  boost::asio::io_context::strand strand_;
  std::set<chat_participant_ptr> participants_;
  enum { max_recent_msgs = 100 };
  chat_message_queue recent_msgs_;
};

//----------------------------------------------------------------

class chat_session
  : public chat_participant, public std::enable_shared_from_this<chat_session>
{
public:
  chat_session(tcp::socket socket, chat_room &room)
    : socket_(std::move(socket)), room_(room)
  {
    
  }

  void start()
  {
    room_.join(shared_from_this());
    do_read_header();
  }

  void deliver(const chat_message &msg)
  {
    bool write_in_progress = !write_msgs_.empty();
    write_msgs_.push_back(msg);
    if (!write_in_progress)
      do_write();
  }
  
private:

  void do_read_header()
  {
    auto self(shared_from_this());
    boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), chat_message::header_length), [this, self](boost::system::error_code error, size_t)
    {
      if (!error && read_msg_.decode_header())
	do_read_body();
      else
	room_.leave(shared_from_this());
    });
  }

  void do_read_body()
  {
    auto self(shared_from_this());
    boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()), [this, self](boost::system::error_code error, size_t)
    {
      if (!error)
	{
	  room_.deliver(read_msg_);
	  do_read_header();
	}
      else
	room_.leave(shared_from_this());
    });
  }

  void do_write()
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()), [this, self](boost::system::error_code error, size_t)
    {
      if (!error)
	{
	  write_msgs_.pop_front();
	  if (!write_msgs_.empty())
	    do_write();
	}
      else
	room_.leave(shared_from_this());
    });
  }
  // boost::asio::io_context::strand strand_;
  tcp::socket socket_;
  chat_room &room_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

//----------------------------------------------------------------

class chat_server
{
public:
  chat_server(boost::asio::io_context &io_context, const tcp::endpoint &endpoint)
    : acceptor_(io_context, endpoint), room_(io_context)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    acceptor_.async_accept([this](boost::system::error_code error, tcp::socket socket)
    {
      if (!error)
	std::make_shared<chat_session>(std::move(socket), room_)->start();
      do_accept();
    });
  }

  tcp::acceptor acceptor_;
  chat_room room_;
};

//----------------------------------------------------------------

int main(int argc, char *argv[])
{
  try {
    // GOOGLE_PROTOBUF_VERIFY_VERSION;
    if (argc < 2)
      {
	std::cerr << "Usage: chat_server <port> [<port> ...]" << std::endl;
	return 1;
      }
    boost::asio::io_context io_context;
    std::list<chat_server> servers;
    for (int i = 1; i < argc; ++i)
      {
	tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
	servers.emplace_back(io_context, endpoint);
      }

    std::vector<std::thread> threadGroup;
    for (int i = 0; i < 5; ++i)
      {
	threadGroup.emplace_back([&io_context](){ io_context.run(); });
      }
    io_context.run();

    for (auto &t : threadGroup)
      t.join();
    
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  // google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
