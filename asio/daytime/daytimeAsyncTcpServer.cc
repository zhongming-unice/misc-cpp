#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp>
#include <ctime>

using boost::asio::ip::tcp;

std::string make_daytime_string()
{
  auto now = std::time(nullptr);
  return std::ctime(&now);
}

class tcp_connection
  : public std::enable_shared_from_this<tcp_connection>
{
public:
  typedef std::shared_ptr<tcp_connection> pointer; 
  static pointer create(boost::asio::io_context &io_context)
  {
    return pointer(new tcp_connection(io_context));
    // constructor is private, so make_shared can't be used
    // return std::make_shared<tcp_connection>(io_context);
  }

  tcp::socket &socket()
  {
    return socket_;
  }

  void start()
  {
    message_ = make_daytime_string();
    auto self = shared_from_this();
    boost::asio::async_write(socket_, boost::asio::buffer(message_),
			     [self = std::move(self)](auto error, auto bytes_transferred){
			       self->handle_write(error, bytes_transferred);
			     });
  }
  
private:
  tcp_connection(boost::asio::io_context &io_context) : socket_(io_context)
  {
    
  }

  void handle_write(const boost::system::error_code &/*error*/,
		    size_t /*bytes_transferred*/)
  {
    
  }
    
  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_context &io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
  {
    start_accept();
  }
  
private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
			   [this, new_connection](auto error){
			     this->handle_accept(new_connection, error);
			   });
  }

  void handle_accept(tcp_connection::pointer new_connection,
		     const boost::system::error_code &error)
  {
    if (!error)
      new_connection->start();
    start_accept();
  }

  boost::asio::io_context &io_context_;
  tcp::acceptor acceptor_;
};


int main(int argc, char *argv[])
{
  try {
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}

