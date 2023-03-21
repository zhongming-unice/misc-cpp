/*
 * Version: @(#)reactor.cpp 1.0.0 30/01/2023
 *
 * Authors: ZHONG Ming
 *
 */

#include "reactor.h"

#include "signal.h"
#include "sys/epoll.h"          


#include <string>
#include <vector>

// ==================== routine ====================
// 每个 reactor 调用的函数
void *reactor_thread_func(void *arg)
{
  signal(SIGPIPE, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);
  Reactor *reactor = (Reactor *)arg;
  int epollfd = reactor->epollfd_;
  std::vector<struct epoll_event> &events = reactor->events_;
  while (1) {
    int nready = epoll_wait(epollfd, events.data(), (int)events.size(), -1);
    if (nready == -1) {
      if (errno == EINTR) continue;
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }
    if (nready == 0) continue;
    if (nready == (int)events.size())
      events.resize(events.size() * 2);
    for (int i = 0; i < nready; ++i) {
      struct epoll_event &ee = events[i];
      // close
      if (ee.events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR)) {
        int close_fd = ee.data.fd;
        close(close_fd);
        epoll_ctl(epollfd, EPOLL_CTL_DEL, close_fd, &ee);
        printf("epoll %d closes %d\n", epollfd, close_fd);
        continue;
      } // end close
      auto &read_queues = reactor->net_server_->read_queues_;
      auto &write_queues = reactor->net_server_->write_queues_;
      // read
      if (ee.events & (EPOLLIN | EPOLLPRI)) {
        std::string msg(READ_LENGTH, 0);
        int n = 0;
        int nread;
#ifdef _EPOLL_ET_MODE_
        while (nread = read(ee.data.fd, &msg[n], READ_LENGTH - 1) > 0)
          n += nread;
#else
        nread = read(ee.data.fd, &msg[0], READ_LENGTH - 1);
#endif
        if (nread == -1 && errno != EAGAIN && errno != EWOULDBLOCK | nread == 0) {
          int close_fd = ee.data.fd;
          close(close_fd);
          epoll_ctl(epollfd, EPOLL_CTL_DEL, close_fd, &ee);
          printf("epoll %d closes %d\n", epollfd, close_fd);
          continue;
        }
        read_queues[ee.data.fd].put(msg);
      } // end read
      // write
      if (ee.events & EPOLLOUT) {
        if (write_queues.count(ee.data.fd) == 0) continue;
        std::string msg_write = write_queues[ee.data.fd].take();
        int msg_write_size = msg_write.size();
        int nwrite;
        int n = msg_write_size;
#ifdef _EPOLL_ET_MODE_
        while (n > 0) {
          nwrite = write(ee.data.fd, &msg_write[msg_write_size - n], n);
          if (nwrite == -1) break;
          n -= nwrite;
        }
#else
        nwrite = write(ee.data.fd, &msg_write[0], msg_write_size);
#endif
        if (nwrite < 0) write_queues[ee.data.fd].put(msg_write);
        ee.events = ee.events & ~EPOLLOUT;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, ee.data.fd, &ee);
      } // end write
    } // end for
  } // end while 1
  return NULL;
}

// ==================== Reactor ====================
Reactor::Reactor()
{
  events_ = decltype(events_)(32);
  epollfd_ = epoll_create1(EPOLL_CLOEXEC);
  if (epollfd_ == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }
}

Reactor::~Reactor()
{
  if (epollfd_) close(epollfd_);
}

void Reactor::Init(NetServer *net_server)
{
  net_server_ = net_server;
}

// ==================== NetServer ====================
NetServer::NetServer()
{
  events_ = decltype(events_)(32);
  reactor_threads_ = decltype(reactor_threads_)(NUM_REACTORS);
  reactors_ = decltype(reactors_)(NUM_REACTORS);
  epollfd_ = epoll_create1(EPOLL_CLOEXEC);
  listenfd_ = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
  if (epollfd_ == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }
  idlefd_ = open("/dev/null", O_RDONLY | O_CLOEXEC);
  if (idlefd_ == -1){
    perror("open");
    exit(EXIT_FAILURE);
  }
}

NetServer::~NetServer()
{
  if (epollfd_) close(epollfd_);
  if (listenfd_) close(listenfd_);
  if (idlefd_) close(idlefd_);
}

void NetServer::ListenPort(uint16_t port)
{
  memset(&addr_, 0, sizeof(addr_));
  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = htonl(INADDR_ANY);
  int on = 1;
  setsockopt(listenfd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
  bind(listenfd_, (struct sockaddr*)&addr_, sizeof(addr_));
  if (listen(listenfd_, 511) < 0) {
     perror("listen");
     return;
  }
  struct epoll_event ee;
  ee.data.fd = listenfd_;
  ee.events = EPOLLIN;
  epoll_ctl(epollfd_, EPOLL_CTL_ADD, listenfd_, &ee);
}

void NetServer::Run()
{
  signal(SIGPIPE, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);
  for (size_t i = 0; i < reactors_.size(); ++i) {
    Reactor &reactor = reactors_[i];
    reactor.Init(this);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&reactor_threads_[i], &attr, reactor_thread_func, (void *)&reactor);
    pthread_attr_destroy(&attr);
  }
  int reactor_index = -1;
  while (1) {
    int nready = epoll_wait(epollfd_, events_.data(), (int)(events_.size()), -1);
    if (nready == -1) {
      if (errno == EINTR) continue;
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }
    if (nready == 0) continue;
    if (nready == (int)events_.size())
      events_.resize(events_.size() * 2);
    struct sockaddr_in peer_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    for (int event_index = 0; event_index < nready; ++event_index) {
      if (events_[event_index].data.fd == listenfd_) {
        int connfd = accept4(listenfd_, (struct sockaddr*)&peer_addr, &addr_len,
                             SOCK_NONBLOCK | SOCK_CLOEXEC);
        if (connfd == -1) {
          if (errno == EMFILE) {
            close(idlefd_);
            idlefd_ = accept(listenfd_, nullptr, nullptr);
            close(idlefd_);
            idlefd_ = open("/dev/null", O_RDONLY | O_CLOEXEC);
          }
          perror("accept4");
          exit(EXIT_FAILURE);
        }
        struct epoll_event ee;
        ee.data.fd = connfd;
        ee.events = EPOLLIN | EPOLL_TRIGGER_MODE;
        reactor_index = (reactor_index + 1) % NUM_REACTORS;
        epoll_ctl(reactors_[reactor_index].epollfd_, EPOLL_CTL_ADD, connfd, &ee);
      } // end accept
    } // end for loop
  } // end while 1
}
