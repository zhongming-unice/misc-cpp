#include <ctime>
#include <iostream>
#include <string>
#include <memory>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
  std::time_t now = time(0);
  return std::ctime(&now);
}

class udp_server
{
public:
  udp_server(boost::asio::io_context &io_context)
    : socket_(io_context, udp::endpoint(udp::v4(), 13))
  {
    start_receive();
  }

private:
  void start_receive()
  {
    socket_.async_receive_from(boost::asio::buffer(recv_buffer_), remote_endpoint_,
			       [this](auto error, auto bytes_transferred)
			       { this->handle_receive(error, bytes_transferred); });
  }

  void handle_receive(const boost::system::error_code &error, std::size_t bytes_transferred)
  {
    if (!error)
      {
	auto message = std::make_shared<std::string>(make_daytime_string());
	socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
			      [this, message](auto error, auto bytes_transferred)
			      { this->handle_send(message, error, bytes_transferred); });

	start_receive();
      }
  }

  void handle_send(std::shared_ptr<std::string>, const boost::system::error_code &, std::size_t)
  {
    
  }
    
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  boost::array<char, 1> recv_buffer_;
};


  
int main(int argc, char *argv[])
{
  try {
    boost::asio::io_context io_context;
    udp_server server(io_context);
    io_context.run();  
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
  
  return 0;
}
