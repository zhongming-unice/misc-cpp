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

int main(int argc, char *argv[])
{
  signal(SIGPIPE, SIG_IGN);
  signal(SIGCHLD, SIG_IGN);

  int idlefd = open("/dev/null", O_RDONLY, O_CLOEXEC);
  int listenfd;

  if ((listenfd = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)) < 0)
    ERR_EXIT("socket");

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(5188);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  int on = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
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
  epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &event);

  EventList events(16);
  struct sockaddr_in peerAddr;
  socklen_t peerLength;
  int connfd;

  int nready;

  while (true)
    {
      nready = epoll_wait(epollfd, events.data(), static_cast<int>(events.size()), -1);
      if (nready == -1)
	{
	  if (errno == EINTR)
	    continue;
	  ERR_EXIT("epoll_wait");
	}

      if (nready == 0)
	continue;

      if ((size_t)nready == events.size())
	events.resize(events.size() * 2);

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
		      close(idlefd);
		      idlefd = accept(listenfd, nullptr, nullptr);
		      close(idlefd);
		      idlefd = open("dev/null", O_RDONLY | O_CLOEXEC);
		      continue;
		    }
		  else
		    ERR_EXIT("accept4");
		}

	      clients.push_back(connfd);
	      event.data.fd = connfd;
	      event.events = EPOLLIN;
	      epoll_ctl(epollfd, EPOLL_CTL_ADD, connfd, &event);
	      std::cout << "ip = " << inet_ntop(peerAddr.sin_addr)
			<< "port = " << ntohs(peerAddr.sin_port) << std::endl;
	      
	    }
	  else if (events[i].events & EPOLLIN)
	    {
	      connfd = events[i].data.fd;
	      if (connfd < 0)
		continue;
	      
	      char buf[1024] = {0};
	      int ret = read(connfd, buf, 1024);
	      if (ret == -1)
		ERR_EXIT("read");
	      if (ret == 0)
		{
		  std::cout << "client close" << std::endl;
		  close(connfd);
		  event = events[i];
		  epoll_ctl(epollfd, EPOLL_CTL_DEL, connfd, &event);
		  clients.erase(std::remove(clients.begin(), clients.end(), connfd),
				clients.end());
		  continue;
		}
	      std::cout << buf;
	      write(connfd, buf, strlen(buf));
	    }
	}
    }

  
  return 0;
}
