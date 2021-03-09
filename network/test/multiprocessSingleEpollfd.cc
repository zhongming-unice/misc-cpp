// test: webbench -c 100 -t 5 http://127.0.0.1:5188/
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <vector>
#include <algorithm>
#include <iostream>

#define ERR_EXIT(m)				\
  do						\
    {						\
      perror(m);				\
      exit(EXIT_FAILURE);			\
    } while(0)

using EventList = std::vector<epoll_event>;

int count = 0;
pid_t pid[10] = {0};

// EPOLLEXCLUSIVE: 只适用于多个线程/进程拥有各自的 epfd, 然后监听同一 listenfd
// SO_REUSEPORT: 只适用于多个 listenfd 绑定同一端口
void process_epoll_wait(int epollfd, int listenfd);

static void siguser_handler(int sig)
{
  printf("pid:%d  count:%d\n", getpid(), count);
  exit(0);
}

static void sigint_handler(int sig)
{
  for (int i = 0; i < 10; i++)
    {
      kill(pid[i], SIGUSR1);
    }
}

int main(int argc, char *argv[])
{
  signal(SIGPIPE, SIG_IGN);
  // signal(SIGCHLD, SIG_IGN);
  signal(SIGINT, sigint_handler);

  if (argc < 2)
    {
      printf("Usage: server [tigger mode] (LT = 0, ET = 1, ONESHOT = 2)");
      exit(1);
    }
  
  int triggerMode = atoi(argv[1]);
  
  int listenfd;

  if ((listenfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)) < 0)
    ERR_EXIT("socket");

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(5188);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int on = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)) < 0)
    ERR_EXIT("setsockopt");
  
  if (bind(listenfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    ERR_EXIT("bind");

  if (listen(listenfd, SOMAXCONN) < 0)
    ERR_EXIT("listen");

  std::vector<int> clients;
  int epollfd;
  epollfd = epoll_create1(EPOLL_CLOEXEC);

  struct epoll_event event;
  event.data.fd = listenfd;
  event.events = EPOLLIN;
  
  if (triggerMode == 1)
    event.events |= EPOLLET;
  if (triggerMode == 2)
    event.events |= EPOLLONESHOT;

  event.events |= EPOLLEXCLUSIVE;

  epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

  int status;
  for (int i = 0; i < 10; ++i)
    {
      int child;
      child = fork();
      if (child == 0)
        {
          signal(SIGUSR1, siguser_handler);

// -------------------------------------------------------------------------------------
          // EPOLLEXCLUSIVE: 只适用于多个线程/进程拥有各自的 epfd, 然后监听同一 listenfd
          // int epollfd2 = epoll_create1(EPOLL_CLOEXEC);
          // epoll_ctl(epollfd2, EPOLL_CTL_ADD, listenfd, &event);
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
          // SO_REUSEPORT: 只适用于多个 listenfd 绑定同一端口
          int epollfd2 = epoll_create1(EPOLL_CLOEXEC);
          int listenfd2 = socket(PF_INET,
                             SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
          setsockopt(listenfd2, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on));
          bind(listenfd2, (sockaddr*)&serverAddr, sizeof(serverAddr));
          listen(listenfd2, SOMAXCONN);
          struct epoll_event event2;
          event2.data.fd = listenfd2;
          event2.events = EPOLLIN;
          // event2.events |= EPOLLEXCLUSIVE;
          if (triggerMode == 1)
            event2.events |= EPOLLET;
          if (triggerMode == 2)
            event2.events |= EPOLLONESHOT;

          // epoll_ctl(epollfd2, EPOLL_CTL_ADD, listenfd2, &event2);
          epoll_ctl(epollfd2, EPOLL_CTL_ADD, listenfd2, &event2);
// -------------------------------------------------------------------------------------

          // process_epoll_wait(epollfd, listenfd);
          // process_epoll_wait(epollfd2, listenfd);
          process_epoll_wait(epollfd2, listenfd2);
        }
      pid[i] = child;
      printf("server:%d  pid:%d\n", i + 1, child);
    }
  wait(&status);
  sleep(1000000);
  close (listenfd);
  return 0;
}


void process_epoll_wait(int epollfd, int listenfd)
{
  int idlefd = open("/dev/null", O_RDONLY, O_CLOEXEC);
  EventList events(128);
  struct sockaddr_in peerAddr;
  socklen_t peerLength;
  int connfd;
  int nready;

  while (true)
    {
      nready = epoll_wait(epollfd, events.data(), static_cast<int>(events.size()), -1);
      // if (nready == -1)
      // 	{
      // 	  if (errno == EINTR)
      // 	    continue;
      // 	  ERR_EXIT("epoll_wait");
      // 	}

      if (nready <= 0)
	continue;

      if ((size_t)nready == events.size())
	events.resize(events.size() * 2);

      // sleep(1);
      for (int i = 0; i < nready; ++i)
	{
	  if (events[i].data.fd == listenfd)
	    {
	      peerLength = sizeof(peerAddr);
	      connfd = accept4(listenfd, (sockaddr*)&peerAddr, &peerLength,
			       SOCK_NONBLOCK | SOCK_CLOEXEC);

              if (connfd == -1)
                {
                  if (errno == EMFILE)
                    {
                      printf("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx:%d\n", getpid());
                      close(idlefd);
                      idlefd = accept(listenfd, nullptr, nullptr);
                      close(idlefd);
                      idlefd = open("dev/null", O_RDONLY | O_CLOEXEC);
                      continue;
                    }
                  else if (errno == EAGAIN)
                    {
                      printf("yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy:%d\n", getpid());
                      continue;
                    }
                  else
                    {
                      printf("------------------------------------------:%d\n", getpid());
                      continue;
                      // ERR_EXIT("accept4");
                    }
                }
              ++count;
              printf("get client:%d\n", getpid());
              close(connfd);
	    }
	  else
	    {
	      printf("kkkkkkkkkkkkkkkkkkkkkkkk:%d\n", getpid());
	    }
	  
	}
    }
}
