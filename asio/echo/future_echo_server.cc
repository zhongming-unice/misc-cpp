#include <stdio.h>
#include <future>
#include <asio.hpp>

std::future<void> echo(asio::ip::tcp::socket socket)
{
  try
    {
      char data[1024];
      while (1)
	{
	  std::future<size_t> f_read = 
	    socket.async_read_some(asio::buffer(data),
				   asio::use_future);
	  auto len = f_read.get();
	  asio::async_write(socket, asio::buffer(data, len),
			    asio::use_future);
	}
    }
  catch (std::exception &e)
    {
      printf("echo exception: %s\n", e.what());
    }
}

std::future<void> listener(asio::io_context &io_context)
{
  asio::ip::tcp::acceptor acceptor(io_context,
				   {asio::ip::tcp::v4(), 9999});
  while (1)
    {
      std::future<asio::ip::tcp::socket> f_socket =
	acceptor.async_accept(asio::use_future);

      auto socket = f_socket.get();
      std::future<void> f_echo
	= std::async(std::launch::async,
		     [&]{ return echo(std::move(socket)).get(); });
      f_echo.get();
    }
}

int main(int argc, char *argv[])
{
  try
    {
      asio::io_context io_context(1);
      asio::signal_set signals(io_context, SIGINT, SIGTERM);
      signals.async_wait([&](auto, auto)
      {
	// signal handler
	io_context.stop();
      });
      std::future<void> f
	= std::async(std::launch::async,
		     [&]{ return listener(io_context).get(); });
      io_context.run();
      f.get();
    }
  catch (std::exception &e)
    {
      printf("exception: %s\n", e.what());  
    }

  return 0;
}
