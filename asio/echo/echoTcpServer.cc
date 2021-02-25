#include <cstdlib>
#include <iostream>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

const int max_length = 1024;

void session(tcp::socket sock)
{
  try
    {
      for (;;)
	{
	  char data[max_length];

	  boost::system::error_code error;
	  size_t length = sock.read_some(boost::asio::buffer(data), error);
	  if(error == boost::asio::error::eof)
	    break;
	  else if (error)
	    throw boost::system::system_error(error);
	  boost::asio::write(sock, boost::asio::buffer(data, length));
	}
    }
  catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;
    }
}

void server(boost::asio::io_context &io_context, unsigned short port)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;)
    {
      std::thread(session, a.accept()).detach();
    }
}

int main(int argc, char *argv[])
{
  try
    {
      if (argc != 2)
	{
	  std::cout << "Usage: blocking_tcp_echo_server <port>" << std::endl;
	  return 1;
	}
      boost::asio::io_context io_context;
      server(io_context, std::atoi(argv[1]));
    }
  catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;
    }
  return 0;
}
