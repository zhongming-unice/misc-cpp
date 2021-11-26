#include "co_routine.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <stack>

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

struct Task {
  coroutine_t *co;
  int fd;
};

static std::stack<Task *> task_stack;
static int g_listen_fd = -1;
static int SetNonBlock(int sockfd) {
  int flags = fcntl(sockfd, F_GETFL, 0);
  flags |= O_NONBLOCK;
  flags |= O_NDELAY;
  return fcntl(sockfd, F_SETFL, flags);
}

static void *IORoutine(void *arg) {
  CoEnableHook();
  Task *task = (Task *)arg;
  char buf[1024];
  while (1) {
    int fd = task->fd;
    if (fd == -1) {
      task_stack.push(task);
      CoYield();
      continue;
    }
    task->fd = -1;
    while (1) {
      pollfd poll_fd = {0};
      poll_fd.fd = fd;
      poll_fd.events = POLLIN | POLLERR | POLLHUP;
      CoPoll(&poll_fd, 1, 1000, nullptr);
      int ret = read(fd, buf, sizeof(buf));
      if (ret > 0) ret = write(fd, buf, ret);
      if (ret > 0 || (ret == -1 && errno == EAGAIN))
        continue;
      close(fd);
      break;
    }
  }
  return nullptr;
}

int CoAccept(int sockfd, sockaddr *addr, socklen_t *addrlen) {
  CoEnableHook();
  fflush(stdout);
  while (1) {
    if (task_stack.empty()) {
      pollfd poll_fd = {0};
      poll_fd.fd = -1;
      poll(&poll_fd, 1, 1000);
      continue;
    }
    int fd = accept4(g_listen_fd, addr, addrlen, 0);
    if (fd < 0) {
      pollfd poll_fd = {0};
      poll_fd.fd = g_listen_fd;
      poll_fd.events = POLLIN | POLLERR | POLLHUP;
      CoPoll(&poll_fd, 1, 1000, nullptr);
      continue;
    }
    if (task_stack.empty()) {
      close(sockfd);
      continue;
    }
    SetNonBlock(sockfd);
    Task *task = task_stack.top();
    task->fd = sockfd;
    task_stack.pop();
    CoResume(task->co);
  }
  return 0;  
}

static void SetAddr(const char *ip, unsigned short port,
                    sockaddr &addr) {
  bzero(&addr, sizeof(addr));
  addr_in = reinterpret_cast<sockaddr_in>(addr);
  addr_in.sin_family = AF_INET;
  addr_in.sin_port = htons(port);
  if (!ip || *ip == '\0' || strcmp(ip, "0") == 0 ||
      strcmp(ip, "0.0.0.0") == 0 || strcmp(ip, "*") == 0)
    addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
  else
    addr_in.sin_addr.s_addr = htonl(ip);
}

static int CreateTcpSocket(const char *ip, const unsigned short port, bool reuse) {
  int fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fd > 0 && port != 0) {
    if (reuse) {
      int on = 1;
      setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    }
    sockaddr addr;
    SetAddr(ip, port, addr);
    int ret = bind(fd, &addr, sizeof(addr));
    if (ret != 0) {
      close(fd);
      return -1;
    }
  }
  return fd;
}

int main(int argc, char *argv[])
{
  if (argc < 3) return -1;
  const char *ip = argv[1];
  int port = atoi(argv[2]);
  g_listen_fd = CreateTcpSocket(ip, port, true);
  
  return 0;
}
