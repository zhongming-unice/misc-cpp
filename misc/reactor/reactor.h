/*
 * Version: @(#)reactor.h 1.0.0 30/01/2023
 *
 * Authors: ZHONG Ming
 *
 */

#ifndef __REACTOR_H__
#define __REACTOR_H__

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <vector>
#include <unordered_map>

#include "blocking_queue.h"

#define NUM_REACTORS 4
#define READ_LENGTH 1024

#ifdef _EPOLL_ET_MODE_
#define EPOLL_TRIGGER_MODE EPOLLET
#else
#define EPOLL_TRIGGER_MODE 0
#endif

struct NetServer;

struct Reactor {
  Reactor();
  virtual ~Reactor();
  void Init(NetServer *);

  NetServer *net_server_;
  int epollfd_;
  std::vector<struct epoll_event> events_;
};

struct NetServer {
  NetServer();
  virtual ~NetServer();

  void ListenPort(uint16_t port);
  void Run();

  int epollfd_;
  int listenfd_;
  int idlefd_;
  struct sockaddr_in addr_;
  std::vector<pthread_t> reactor_threads_;
  std::vector<Reactor> reactors_;
  std::vector<struct epoll_event> events_;
  std::unordered_map<int, BlockingQueue<std::string> > read_queues_;
  std::unordered_map<int, BlockingQueue<std::string> > write_queues_;
};

#endif
