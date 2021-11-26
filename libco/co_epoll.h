#ifndef __CO_EPOLL_H__
#define __CO_EPOLL_H__

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <time.h>

// typedef union epoll_data {
//   void        *ptr;
//   int          fd;
//   uint32_t     u32;
//   uint64_t     u64;
// } epoll_data_t;

// struct epoll_event {
//   uint32_t     events;      /* Epoll events */
//   epoll_data_t data;        /* User data variable */
// };

struct co_epoll_res {
  int size;
  struct epoll_event *events;
};

int CoEpollWait(int epfd, co_epoll_res *res, int maxevents, int timeout);
int CoEpollCtl(int epfd, int op, int fd, epoll_event *event);
int CoEpollCreate(int size);
co_epoll_res *CoEpollResAlloc(int size);
void CoEpollResFree(co_epoll_res *res);

#endif  // __CO_EPOLL_H__
