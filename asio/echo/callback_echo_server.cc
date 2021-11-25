#include <stdio.h>
#include <memory>
#include <utility>
#include "asio.hpp"

// CRTP
class Session : public std::enable_shared_from_this<Session>
{
public:
  Session(asio::ip::tcp::socket socket)
    : socket_(std::move(socket)) {}

  void start()
  {
    do_read();
  }
  
private:
  void do_read()
  {
    printf("%s\n", socket_.remote_endpoint(ec).address().to_string().c_str());
    auto self(shared_from_this());
    // async_read_some(asio::error_code, std::size_t)>::return_type
    // async_read_some(const MutableBufferSequence &buffers,
    // const ReadHandler &handler)
    socket_.async_read_some(asio::buffer(data_, maxLen),
			    [this, self]
			    (std::error_code ec, size_t len)
			    {
			      if (!ec)
				do_write(len);
			    });
  }

  void do_write(size_t len)
  {
    auto self(shared_from_this());
    // async_write(asio::error_code, std::size_t)>::return_type
    // async_write(AsyncWriteStream &s, DynamicBuffer_v2 buffers,
    // CompletionCondition completion_condition,
    // const WriteHandler &handler,
    // typename enable_if<is_dynamic_buffer_v2<DynamicBuffer_v2>
    // ::value>::type * = 0)
    asio::async_write(socket_, asio::buffer(data_, len),
		      [this, self](std::error_code ec, size_t)
		      {
			if (!ec)
			  do_read();
		      });
  }
  
  asio::ip::tcp::socket socket_;
  enum { maxLen = 1024 };
  char data_[maxLen];
};

class Server
{
public:
  Server(asio::io_context &io_context, short port)
    : acceptor_(io_context,
		asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
  {
    do_accept();
  }

private:
  void do_accept()
  {
    // async_accept(asio::error_code)>::return_type
    // async_accept(basic_socket<protocol_type, Executor1> &peer,
    // endpoint_type &peer_endpoint, const AcceptHandler &handler)
    acceptor_.async_accept([this](std::error_code ec,
				  asio::ip::tcp::socket socket)
    {
      if (!ec)
	std::make_shared<Session>(std::move(socket))->start();
      do_accept();
    });
  }

  asio::ip::tcp::acceptor acceptor_;
};

int main(int argc, char *argv[])
{
  try
    {
      asio::io_context io_context;
      Server server(io_context, 9999);
      io_context.run();
    }
  catch (std::exception &e)
    {
      printf("exception %s\n", e.what());
    }
  return 0;
}
