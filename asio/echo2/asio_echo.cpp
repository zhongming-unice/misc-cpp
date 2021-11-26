#include "asio_echo.h"

ReturnObject Echo(asio::ip::tcp::socket socket) {
  std::array<char, 1024> buffer;
  while (1) {
    auto result = co_await ReadAwaiter(socket, asio::buffer(buffer));
    if (!result) {
      fprintf(stderr, "read error: %s\n", result.ErrorCode().message().c_str());
      break;
    }
    result = co_await WriteAwaiter(socket, asio::buffer(buffer.data(), result.Get()));
    if (!result) {
      fprintf(stderr, "write error: %s\n", result.ErrorCode().message().c_str());
      break;
    }
  }
}

ReturnObject Accept(asio::ip::tcp::acceptor acceptor) {
  while (1) {
    Result<asio::ip::tcp::socket> result = co_await AcceptAwaiter(acceptor);
    if (result) {
      Echo(std::move(result.Get()));
    } else {
      fprintf(stderr, "accept error: %s\n", result.ErrorCode().message().c_str());
      break;
    }
  }
}

int main(int argc, char *argv[])
{
  try {
    asio::io_context io_context(1);
    asio::signal_set signals(io_context, SIGINT, SIGTERM);
    signals.async_wait([&](auto, auto) { io_context.stop(); });
    asio::ip::tcp::acceptor acceptor(io_context,
                                     asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 9999));
    Accept(std::move(acceptor));
    io_context.run();    
  } catch (std::exception &e) {
    printf("exception: %s \n", e.what());
  }
  
  return 0;
}
