#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include "chatMessage.h"

using boost::asio::ip::tcp;

using chat_message_queue = std::deque<chat_message>;

class chat_client
{
public:
  chat_client(boost::asio::io_context &io_context,
	      const tcp::resolver::results_type &endpoint)
    : io_context_(io_context), socket_(io_context)
  {
    do_connect(endpoint);
  }

  void write(const chat_message &msg)
  {
    boost::asio::post(io_context_,
		      [this, msg]()
		      {
			bool write_in_progress = !write_msgs_.empty();
			write_msgs_.push_back(msg);
			if (!write_in_progress)
			  do_write();
		      });
  }

  void close()
  {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
  }
  
private:
  void do_connect(const tcp::resolver::results_type &endpoint)
  {
    boost::asio::async_connect(socket_, endpoint,
			       [this](boost::system::error_code error, tcp::endpoint)
			       {
				 if (!error)
				   do_read_header();
			       });
  }

  void do_read_header()
  {
    boost::asio::async_read(socket_,
			    boost::asio::buffer(read_msg_.data(), chat_message::header_length),
			    [this](boost::system::error_code error, size_t)
			    {
			      if (!error && read_msg_.decode_header())
				do_read_body();
			      else
				socket_.close();
			    });
			   
  }

  void do_read_body()
  {
    boost::asio::async_read(socket_,
			    boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
			    [this](boost::system::error_code error, size_t)
			    {
			      if (!error)
				{
				  std::cout.write(read_msg_.body(), read_msg_.body_length());
				  std::cout << std::endl;
				  do_read_header();
				}
			      else
				socket_.close();
			    });
  }

  void do_write()
  {
    boost::asio::async_write(socket_,
			     boost::asio::buffer(write_msgs_.front().data(),
						 write_msgs_.front().length()),
			     [this](boost::system::error_code error, size_t)
			     {
			       if (!error)
				 {
				   write_msgs_.pop_front();
				   if (!write_msgs_.empty())
				     do_write();
				 }
			       else
				 socket_.close();
			     });
  }
  
  boost::asio::io_context &io_context_;
  tcp::socket socket_;
  chat_message read_msg_;
  chat_message_queue write_msgs_;
};

int main(int argc, char *argv[])
{
  try
    {
      if (argc != 3)
	{
	  std::cerr << "Usage: chat_client <host> <port>" << std::endl;
	  return 1;
	}
      boost::asio::io_context io_context;

      tcp::resolver resolver(io_context);
      auto endpoint = resolver.resolve(argv[1], argv[2]);
      chat_client c(io_context, endpoint);

      std::thread t([&io_context](){ io_context.run(); });

      char line[chat_message::max_body_length + 1];
      while (std::cin.getline(line, chat_message::max_body_length + 1))
	{
	  chat_message msg;
	  msg.body_length(std::strlen(line));
	  std::memcpy(msg.body(), line, msg.body_length());
	  msg.encode_header();
	  c.write(msg);
	}

      c.close();
      t.join();
      
    }
  catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;
    }

  
  return 0;
}


