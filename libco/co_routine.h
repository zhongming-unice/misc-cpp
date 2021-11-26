#ifndef __CO_ROUTINE_H__
#define __CO_ROUTINE_H__

#include <stdint.h>
#include <sys/poll.h>
#include <pthread.h>

#include <co_ctx.h>
#include <co_epoll.h>

struct coroutine_t;
struct co_epoll_t;
struct stack_mem_t;
struct timing_wheel_t;
struct timeout_item_t;
struct timeout_item_link_t;
struct co_poll_item_t;
struct co_cond_t;

typedef void (*on_prepare_pfn)(timeout_item_t *, epoll_event &, timeout_item_link_t *);
typedef void (*on_process_pfn)(timeout_item_t *);
typedef int (*co_eventloop_pfn)(void *);
typedef void *(*coroutine_pfn)(void *);
typedef int (*poll_pfn)(pollfd fds[], nfds_t nfds, int timeout);

struct co_spec_t {
  void *value;  
};

struct co_env_t {
  coroutine_t *call_stack[128];
  int call_stack_size;
  co_epoll_t *epoll;
  // 在不使用共享栈模式时 pending_co 和 ocupy_co 都是空指针
  coroutine_t *pending_co;
  coroutine_t *occupy_co;
};

struct coroutine_t {
  co_env_t *env;
  coroutine_pfn pfn;
  void *args;
  co_ctx ctx;
  char start;
  char end;
  char is_main;
  char enable_sys_hook;
  char is_share_stack;
  void *pv_env;
  stack_mem_t *stack_mem;
  char *stack_sp;               // 栈低地址
  unsigned int save_size;
  char *save_buffer;
  co_spec_t spec[1024];
};

// int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
struct co_epoll_t {
  int epoll_fd;
  static const int EPOLL_SIZE = 10240; // the maxevents argument of epoll_wait(2)
  timing_wheel_t *timer;
  timeout_item_link_t *timeout_list;
  timeout_item_link_t *active_list;
  co_epoll_res *result;         // contains the events argument of epoll_wait(2)
};

// Timing Wheel
struct timing_wheel_t {
  timeout_item_link_t *link;    // slot_size 个 timeout_item_link_t
  int slot_size;                // 60000
  unsigned long long start;
  long long start_idx;
};

struct timeout_item_t {
  enum {
    MAX_TIMEOUT = 40000
  };
  timeout_item_t *prev;
  timeout_item_t *next;
  timeout_item_link_t *link;
  unsigned long long expire_time;
  on_prepare_pfn prepare_pfn;
  on_process_pfn process_pfn;
  void *args;                   // routine
  bool is_timeout;
};

struct timeout_item_link_t {
  timeout_item_t *head;
  timeout_item_t *tail;
};

struct co_poll_t : public timeout_item_t {
  pollfd *fds;
  nfds_t nfds;                  // unsigned long int
  co_poll_item_t *poll_items;
  int all_event_detach;
  int epoll_fd;
  int raise_cnt;
};

struct co_poll_item_t : public timeout_item_t {
  pollfd *self;
  co_poll_t *poll;
  epoll_event event;
};

struct stack_mem_t {
  coroutine_t *occupy_co;
  int stack_size;
  char *stack_bp;               // 栈高地址
  char *stack_buffer;
};

struct share_stack_t {
  unsigned int alloc_idx;
  int stack_size;               // size of each stack
  int count;                    // number of stacks
  stack_mem_t **stack_array;
};

struct co_attr_t {
  int stack_size;
  // 在不使用共享栈模式时 share_stack 是空指针
  share_stack_t *share_stack;
  co_attr_t() {
    stack_size = 128 * 1024;
    share_stack = nullptr;
  }
}__attribute__((packed));

struct co_cond_item_t {
  co_cond_item_t *prev;
  co_cond_item_t *next;
  co_cond_t *link;
  timeout_item_t timeout;
};
  
struct co_cond_t {
  co_cond_item_t *head;
  co_cond_item_t *tail;
};

  
void CurrThreadEnvInit();
co_env_t *CurrThreadEnvGet();

int CoCreate(coroutine_t **ppco, const co_attr_t *attr, coroutine_pfn pfn, void *args);
void CoResume(coroutine_t *co);
void CoYield();
void CoFree(coroutine_t *co);
void CoRelease(coroutine_t *co);
void CoReset(coroutine_t *co);
coroutine_t *CoSelf();
int CoPoll(co_poll_t *ctx, pollfd fds[], nfds_t nfds, int timeout);
void CoEventloop(co_eventloop_pfn pfn, void *args);
void CoSwap(coroutine_t *curr, coroutine_t *pending);

stack_mem_t *StackMemAlloc(unsigned int stack_size);
stack_mem_t *StackMemGet(share_stack_t *share_stack);
share_stack_t *ShareStackAlloc(int count, int stack_size);
void StackBufferSave(coroutine_t *co);

timing_wheel_t *TimerAlloc(int size);
void TimerFree(timing_wheel_t *timer);
int TimerItemAdd(timing_wheel_t *timer, timeout_item_t *item, unsigned long long now);

co_epoll_t *EpollAlloc();
void EpollFree(co_epoll_t *epoll);

int CoPoll(pollfd fds[], nfds_t nfds, int timeout, poll_pfn pollfunc);
void OnProcessEvent(timeout_item_t *item);
void OnPollPrepare(timeout_item_t *timeout_item, epoll_event &event,
                   timeout_item_link_t *active);

// ------------- inline functions -------------
template<class Link, class Item>
inline void RemoveFromLink(Item *item) {
  Link *link = item->link;
  if (!link) return;
  
  if (item == link->head) {
    link->head = item->next;
    if (link->head) link->head->prev = nullptr;
  } else if (item->prev) {
    item->prev->next = item->next;
  }

  if (item == link->tail) {
    link->tail = item->prev;
    if (link->tail) link->tail->next = nullptr;
  } else {
    item->next->prev = item->prev;
  }
  item->prev = item->next = nullptr;
  item->link = nullptr;
}

inline void TimeoutRemoveFromLink(timeout_item_t *item) {
  RemoveFromLink<timeout_item_link_t, timeout_item_t>(item);
}

template<class Link, class Item>
inline void TailAdd(Link *link, Item *item) {
  if (item->link) return;
  item->link = link;
  if (link->tail) {
    link->tail->next = item;
    item->next = nullptr;
    item->prev = link->tail;
    link->tail = item;
  } else {
    link->head = link->tail = item;
    item->next = item->prev = nullptr;
  }
}
  
inline void TimeoutLinkTailAdd(timeout_item_link_t *link, timeout_item_t *item) {
  TailAdd<timeout_item_link_t, timeout_item_t>(link, item);
}

template<class Link, class Item>
inline void HeadPop(Link *link) {
  if (!link->head) return;
  Item *head = link->head;
  if (link->head == link->tail)
    link->head = link->tail = nullptr;
  else
    link->head = link->head->next;
  head->prev = head->next = nullptr;
  head->link = nullptr;
  if (link->head) link->head->prev = nullptr;
}

inline void TimeoutLinkHeadPop(timeout_item_link_t *link) {
  HeadPop<timeout_item_link_t, timeout_item_t>(link);
}

template<class Link, class Item>
inline void LinkAppend(Link *lhs, Link *rhs) {
  if (!rhs->head) return;
  Item *rhs_head = rhs->head;
  while (rhs_head) {
    rhs_head->link = lhs;
    rhs_head = rhs_head->next;
  }
  rhs_head = rhs->head;
  if (lhs->tail) {
    lhs->tail->next = rhs_head;
    rhs_head->prev = lhs->tail;
    lhs->tail = rhs->tail;
  } else {
    lhs->head = rhs->head;
    lhs->tail = rhs->tail;
  }
  rhs->head = rhs->tail = nullptr;
}

inline void TimeoutLinkAppend(timeout_item_link_t *lhs, timeout_item_link_t *rhs) {
  LinkAppend<timeout_item_link_t, timeout_item_t>(lhs, rhs);
}

inline void TimeoutLinkAllTake(timing_wheel_t *timer, unsigned long long now,
                           timeout_item_link_t *link) {
  if (timer->start == 0) {
    timer->start = now;
    timer->start_idx = 0;
  }
  if (now < timer->start) return;
  int cnt = now - timer->start + 1;
  if (cnt > timer->slot_size)
    cnt = timer->slot_size;
  for (int i = 0; i < cnt; ++i) {
    int idx = (timer->start_idx + i) % timer->slot_size;
    TimeoutLinkAppend(link, timer->link + idx);
  }
  timer->start = now;
  timer->start_idx += cnt - 1;
}

int CoCondSignal(co_cond_t *cond); // notify one
int CoCondBroadcast(co_cond_t *cond); // notify all
int CoCondTimewait(co_cond_t *cond, int timeout); // wait for

void CoEnableHook();
void SetEnvList(const char *name[], size_t count);

#endif  // __CO_ROUTINE_H__
