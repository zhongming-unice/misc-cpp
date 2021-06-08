#include <asio/co_spawn.hpp>
#include <asio/detached.hpp>
#include <asio/io_context.hpp>
#include <asio/ip/tcp.hpp>
#include <asio/signal_set.hpp>
#include <asio/write.hpp>
#include <stdio.h>

// #if defined(ASIO_ENABLE_HANDLER_TRACKING)
// # define use_awaitable \
//   asio::use_awaitable_t(__FILE__, __LINE__, __PRETTY_FUNCTION__)
// #endif

asio::awaitable<void> echo(asio::ip::tcp::socket socket)
{
  try
    {
      char data[1024];
      while (1)
	{
	  size_t n = co_await
	    socket.async_read_some(asio::buffer(data),
				   asio::use_awaitable);
	  co_await async_write(socket, asio::buffer(data, n),
			       asio::use_awaitable);
	}
    }
  catch (std::exception &e)
    {
      printf("echo exception: %s\n", e.what());  
    }
}

asio::awaitable<void> listener()
{
  auto executor = co_await asio::this_coro::executor;
  asio::ip::tcp::acceptor acceptor(executor,
				   {asio::ip::tcp::v4(), 9999});
  while (1)
    {
      asio::ip::tcp::socket socket = co_await
	acceptor.async_accept(asio::use_awaitable);
      
      asio::co_spawn(executor, echo(std::move(socket)),
		     asio::detached);
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
      asio::co_spawn(io_context, listener(), asio::detached);
      io_context.run();
    }
  catch (std::exception &e)
    {
      printf("exception: %s\n", e.what());  
    }

  return 0;
}
