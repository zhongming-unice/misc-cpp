#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 10 };

int main(int argc, char *argv[])
{
  try
    {
      if (argc != 3)
	{
	  std::cerr << "Usage: blocking_tcp_echo_client <host> <port>" << std::endl;
	  return 1;
	}
      boost::asio::io_context io_context;
      tcp::socket s(io_context);
      tcp::resolver resolver(io_context);
      boost::asio::connect(s, resolver.resolve(argv[1], argv[2]));

      std::cout << "enter message: " << std::endl;
      char request[max_length];
      std::cin.getline(request, max_length);
      size_t request_length = std::strlen(request);
      boost::asio::write(s, boost::asio::buffer(request, request_length));

      char reply[max_length];
      size_t reply_length = boost::asio::read(s, boost::asio::buffer(reply, request_length));
      std::cout << "reply is: " << std::endl;
      std::cout.write(reply, reply_length);
      std::cout << std::endl;
    }
  catch (std::exception &e)
    {
      std::cerr << e.what() << std::endl;    
    }
  
  return 0;
}
