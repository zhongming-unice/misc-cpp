#include "co_epoll.h"

#include <stdio.h>
#include <errno.h>

int CoEpollWait(int epfd, co_epoll_res *res, int maxevents, int timeout) {
  return epoll_wait(epfd, res->events, maxevents, timeout);
}

int CoEpollCtl(int epfd, int op, int fd, epoll_event *event) {
  return epoll_ctl(epfd, op, fd, event);
}

int CoEpollCreate(int size) {
  return epoll_create(size);
}

co_epoll_res *CoEpollResAlloc(int size) {
  co_epoll_res *ptr = (co_epoll_res *)calloc(1, sizeof(co_epoll_res));
  ptr->size = size;
  ptr->events = (epoll_event *)calloc(1, size * sizeof(epoll_event));
  return ptr;
}

void CoEpollResFree(co_epoll_res *ptr) {
  if (!ptr) return;
  if (ptr->events) free(ptr->events);
  free(ptr);
}
