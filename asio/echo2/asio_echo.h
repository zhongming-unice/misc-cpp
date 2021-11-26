#pragma once

#include <type_traits>          // for enable_if_t
#include <variant>              // for variant
#include <concepts>             // for convertible_to
#include <exception>            // for exception_ptr
#include <coroutine>

#include <stdio.h>

#include <asio.hpp>

template <class T, typename = std::enable_if_t<!std::is_same_v<T, std::error_code>>>
struct Result {
  Result() = default;
  template<std::convertible_to<T> From>
  Result(From &&from) : value_(std::forward<From>(from)) {}
  Result(std::error_code ec) : value_(ec) {}
  operator bool() const { return std::holds_alternative<T>(value_); }
  T &Get() { return std::get<T>(value_); }
  std::error_code ErrorCode() {
    if (auto *ec = std::get_if<std::error_code>(&value_)) return *ec;
    else return {};
  }
  std::variant<std::error_code, T> value_;
};


// template<class T>
// struct ReturnObject {
//   struct promise_type {
//     ReturnObject get_return_object() {
//       return { .handle_ = std::coroutine_handle<promise_type>::from_promise(*this) };
//     }
//     std::suspend_never initial_suspend() { return {}; }
//     std::suspend_always final_suspend() noexcept { return {}; }
//     void unhandled_exception() { exception_ = std::current_exception(); }
//     template<std::convertible_to<T> From>
//     std::suspend_always yield_value(From &&from) {
//       value_ = std::forward<From>(from);
//       return {};
//     }
//     void return_void() {}
    
//     T value_;
//     std::exception_ptr exception_;
//   };

//   std::coroutine_handle<promise_type> handle_;
// };

struct ReturnObject {
  struct promise_type {
    ReturnObject get_return_object() {
      return { .handle_ = std::coroutine_handle<promise_type>::from_promise(*this) };
    }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_always final_suspend() noexcept { return {}; }
    void unhandled_exception() { exception_ = std::current_exception(); }
    std::suspend_always yield_value() { return {}; }
    void return_void() {}
    
    std::exception_ptr exception_;
  };

  std::coroutine_handle<promise_type> handle_;
};

struct AcceptAwaiter {
  AcceptAwaiter(asio::ip::tcp::acceptor &acceptor)
    : acceptor_(acceptor), socket_(acceptor.get_executor()) {}
  bool await_ready() const { return false; }
  
  void await_suspend(std::coroutine_handle<> handle) {
    acceptor_.async_accept(socket_, [this, handle](std::error_code ec) mutable {
                                      if (ec) result_ = ec;
                                      else result_ = std::move(socket_);
                                      handle.resume();
                                    });
  }

  Result<asio::ip::tcp::socket> await_resume() { return std::move(result_); }
  
  asio::ip::tcp::acceptor &acceptor_;
  asio::ip::tcp::socket socket_;
  Result<asio::ip::tcp::socket> result_;
};

struct IOAwaiter {
  bool await_ready() const { return false; }
  Result<std::size_t> await_resume() { return std::move(result_); }
  
  struct handle_done {
    void operator()(std::error_code ec, std::size_t bytes_transferred) {
      if (ec) io_awaiter_->result_ = ec;
      else io_awaiter_->result_ = bytes_transferred;
      handle_.resume();
    }
    IOAwaiter *io_awaiter_;
    std::coroutine_handle<> handle_;
  };
  
  Result<std::size_t> result_;
};

struct ReadAwaiter : IOAwaiter {
  ReadAwaiter(asio::ip::tcp::socket &socket, asio::mutable_buffer buffer)
    : socket_(socket), buffer_(buffer) {}
  void await_suspend(std::coroutine_handle<> handle) {
    socket_.async_read_some(asio::buffer(buffer_), handle_done{this, handle});
  }
  asio::ip::tcp::socket &socket_;
  asio::mutable_buffer buffer_;
};

struct WriteAwaiter : IOAwaiter {
  WriteAwaiter(asio::ip::tcp::socket &socket, asio::const_buffer buffer)
    : socket_(socket), buffer_(buffer) {}
  void await_suspend(std::coroutine_handle<> handle) {
    socket_.async_write_some(asio::buffer(buffer_), handle_done{this, handle});
  }
  asio::ip::tcp::socket &socket_;
  asio::const_buffer buffer_;
};
