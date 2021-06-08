#include <stdio.h>
#include <memory>
#include <vector>
#include <thread>
#include <utility>
#include "asio.hpp"

// CRTP
class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(asio::io_context &io_context)
    : socket_(io_context), strand_(io_context) {}

  void start()
  {
    do_read();
  }

  asio::ip::tcp::socket &socket() { return socket_; }
  
private:
  void do_read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(asio::buffer(data_, maxLen),
			    asio::bind_executor(strand_,
						[this, self]
			    (std::error_code ec, size_t len)
			    {
			      if (!ec)
				do_write(len);
			    }));
  }

  void do_write(size_t len)
  {
    auto self(shared_from_this());
    asio::async_write(socket_, asio::buffer(data_, len),
		      asio::bind_executor(strand_,
		       [this, self](std::error_code ec, size_t)
		      {
			if (!ec)
			  do_read();
		      }));
  }

  asio::io_context::strand strand_;
  asio::ip::tcp::socket socket_;
  enum { maxLen = 1024 };
  char data_[maxLen];
};

class Server
{
public:
  Server(asio::io_context &io_context, short port)
    : acceptor_(io_context,
		asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port)),
      io_context_(io_context)
  {
    do_accept();
  }

private:
  void do_accept()
  {
    auto session = std::make_shared<Session>(io_context_);
    acceptor_.async_accept(session->socket(),
			   [this, session](std::error_code ec)
			   {
			     if (!ec)
			       session->start();
			     do_accept();
			   });
  }

  asio::io_context &io_context_;
  asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char *argv[])
{
  try
    {
      asio::io_context io_context;
      Server server(io_context, 9999);
      auto thread_num = std::thread::hardware_concurrency();
      printf("thread number is: %d\n", thread_num);
      std::vector<std::thread> thread_pool;
      for (int i = 0; i < thread_num; ++i)
	{
	  thread_pool.emplace_back([&io_context](){ io_context.run(); });
	}

      for (auto &t : thread_pool)
	t.join();
    }
  catch (std::exception &e)
    {
      printf("exception %s\n", e.what());
    }
  return 0;
}
