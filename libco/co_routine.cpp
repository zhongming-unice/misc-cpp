#include "co_routine.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <map>
#include <string>

#include "co_epoll.h"

extern "C" {
  extern void co_ctx_swap(co_ctx *, co_ctx *) asm("co_ctx_swap");
};

// ------------- thread-local storage -------------
static __thread co_env_t *g_co_env = nullptr;

// ------------- static functions -------------
static int CoFunc(coroutine_t *co, void *) {
  if (co->pfn) co->pfn(co->args);
  co->end = 1;
  CoYield();
  return 0;
}

static unsigned long long TickMSGet() {
  timeval now = {0};
  gettimeofday(&now, nullptr);
  return 1000 * now.tv_sec + now.tv_usec / 1000; 
}

// ------------- conversion between poll and epoll -------------
static uint32_t PollEvent2Epoll(short events) {
  uint32_t e = 0;
  if (events & POLLIN) e |= EPOLLIN;
  if (events & POLLOUT) e |= EPOLLOUT;
  if (events & POLLHUP) e |= EPOLLHUP;
  if (events & POLLERR) e |= EPOLLERR;
  if (events & POLLRDNORM) e |= EPOLLRDNORM; // Equivalent to POLLIN
  if (events & POLLWRNORM) e |= EPOLLWRNORM; // Equivalent to POLLOUT
  return e;
}

static short EpollEvent2Poll(uint32_t events) {
  short e = 0;
  if (events & EPOLLIN) e |= POLLIN;
  if (events & EPOLLOUT) e |= POLLOUT;
  if (events & EPOLLHUP) e |= POLLHUP;
  if (events & EPOLLERR) e |= POLLERR;
  if (events & EPOLLRDNORM) e |= POLLRDNORM; // Equivalent to EPOLLIN
  if (events & EPOLLWRNORM) e |= POLLWRNORM; // Equivalent to EPOLLOUT
  return e;
}

// ------------- thread-local coroutine environment -------------
void CurrThreadEnvInit() {
  g_co_env = (co_env_t *)calloc(1, sizeof(co_env_t));
  co_env_t *env = g_co_env;
  env->call_stack_size = 0;
  // create main coroutine
  coroutine_t *self = nullptr;
  CoCreate(&self, nullptr, nullptr, nullptr);
  self->is_main = 1;
  env->pending_co = nullptr;
  env->occupy_co = nullptr;
  CoCtxInit(&self->ctx);
  env->call_stack[env->call_stack_size++] = self;
  co_epoll_t *epoll = EpollAlloc();
  env->epoll = epoll;
}

co_env_t *CurrThreadEnvGet() {
  if (!g_co_env)
    CurrThreadEnvInit();
  return g_co_env;
}

// ------------- coroutine -------------
int CoCreate(coroutine_t **ppco, const co_attr_t *attr, coroutine_pfn pfn, void *args) {
  co_env_t *env = CurrThreadEnvGet();
  co_attr_t attr2;
  if (attr)
    memcpy(&attr2, attr, sizeof(attr2));
  if (attr2.stack_size <= 0)
    attr2.stack_size = 128 * 1024;
  else if (attr2.stack_size > 8 * 1024 * 1024)
    attr2.stack_size = 8 * 1024 * 1024;
  // 4 kb 对齐
  if (attr2.stack_size & 0xFFF) {
    attr2.stack_size &= ~0xFFF;
    attr2.stack_size += 0x1000;
  }
  coroutine_t *co = (coroutine_t *)calloc(1, sizeof(coroutine_t));
  co->env = env;
  co->pfn = pfn;
  co->args = args;
  stack_mem_t *stack_mem = nullptr;
  if (attr2.share_stack) {
    stack_mem = StackMemGet(attr2.share_stack);
    attr2.stack_size = attr2.share_stack->stack_size;
  } else {
    stack_mem = StackMemAlloc(attr2.stack_size);
  }
  co->stack_mem = stack_mem;
  co->ctx.ss_sp = stack_mem->stack_buffer;
  co->ctx.ss_size = attr2.stack_size;
  co->start = 0;
  co->end = 0;
  co->is_main = 0;
  co->enable_sys_hook = 0;
  co->is_share_stack = attr2.share_stack != nullptr;
  co->save_size = 0;
  co->save_buffer = nullptr;
  
  *ppco = co;
  return 0;
}

void CoResume(coroutine_t *co) {
  co_env_t *env = co->env;
  coroutine_t *curr = env->call_stack[env->call_stack_size - 1];
  if (!co->start) {
    CoCtxMake(&co->ctx, (co_ctx_pfn)CoFunc, co, 0);
    co->start = 1;
  }
  env->call_stack[env->call_stack_size++] = co;
  CoSwap(curr, co);
}

void CoYield() {
  co_env_t *env = CurrThreadEnvGet();
  coroutine_t *last = env->call_stack[env->call_stack_size - 2];
  coroutine_t *curr = env->call_stack[env->call_stack_size - 1];
  env->call_stack_size--;
  CoSwap(curr, last);  
}

void CoFree(coroutine_t *co) {
  if (!co->is_share_stack) {
    free(co->stack_mem->stack_buffer);
    free(co->stack_mem);
  } else {
    if (co->save_buffer)
      free(co->save_buffer);
    if (co->stack_mem->occupy_co == co)
      co->stack_mem->occupy_co = nullptr;
  }
  
  free(co);
}

void CoRelease(coroutine_t *co) {
  CoFree(co);
}

void CoReset(coroutine_t *co) {
  if (!co->start || co->is_main) return;
  co->start = 0;
  co->end = 0;
  
  if (co->save_buffer) {
    free(co->save_buffer);
    co->save_buffer = nullptr;
    co->save_size = 0;
  }

  if (co->stack_mem->occupy_co == co)
    co->stack_mem->occupy_co = nullptr;
}

coroutine_t *CoSelf() {
  co_env_t *env = CurrThreadEnvGet();
  return env->call_stack[env->call_stack_size - 1];
}

void CoEventloop(co_eventloop_pfn pfn, void *args) {
  co_epoll_t *epoll = CurrThreadEnvGet()->epoll;
  if (!epoll->result)
    epoll->result = CoEpollResAlloc(co_epoll_t::EPOLL_SIZE);
  while (1) {
    // block 1 millisecond
    int ret = epoll_wait(epoll->epoll_fd, epoll->result->events,
                         co_epoll_t::EPOLL_SIZE, 1);
    memset(epoll->timeout_list, 0, sizeof(timeout_item_link_t));
    for (int i = 0; i < ret; i++) {
      timeout_item_t *item = (timeout_item_t *)epoll->result->events[i].data.ptr;
      if (item->prepare_pfn)
        item->prepare_pfn(item, epoll->result->events[i], epoll->active_list);
      else
        TimeoutLinkTailAdd(epoll->active_list, item);
    }
    unsigned long long now = TickMSGet();
    TimeoutLinkAllTake(epoll->timer, now, epoll->timeout_list);
    timeout_item_t *head = epoll->timeout_list->head;
    while (head) {
      head->is_timeout = true;
      head = head->next;
    }
    TimeoutLinkAppend(epoll->active_list, epoll->timeout_list);
    head = epoll->active_list->head;
    
    while (head) {
      TimeoutLinkHeadPop(epoll->active_list);
      if (head->is_timeout && now < head->expire_time) {
        int ret = TimerItemAdd(epoll->timer, head, now);
        // if no error
        if (!ret) {
          head->is_timeout = false;
          head = epoll->active_list->head;
          continue;
        }
      }
      if (head->process_pfn)
        head->process_pfn(head);
      head = epoll->active_list->head;
    } // end while (head)
    
    if (pfn && pfn(args) == -1) break;
      
  } // end while (1)
  
}

void CoSwap(coroutine_t *curr, coroutine_t *pending) {
  co_env_t *env = CurrThreadEnvGet();
  char sp;
  curr->stack_sp = &sp;
  if (!pending->is_share_stack) {
    env->pending_co = nullptr;
    env->occupy_co = nullptr;
  } else {
    env->pending_co = pending;
    // get last occupy coroutine on the same stack mem
    coroutine_t *occupy = pending->stack_mem->occupy_co;
    // set pending co to occupy stack mem
    pending->stack_mem->occupy_co = pending;

    env->occupy_co = occupy;
    if (occupy && occupy != pending)
      StackBufferSave(occupy);
  }
  co_ctx_swap(&(curr->ctx), &(pending->ctx));

  // stack buffer may be overwritten, so get again
  co_env_t *curr_env = CurrThreadEnvGet();
  coroutine_t *update_occupy_co = curr_env->occupy_co;
  coroutine_t *update_pending_co = curr_env->pending_co;

  if (update_occupy_co && update_pending_co &&
      update_occupy_co != update_pending_co) {
    if (update_pending_co->save_buffer && update_pending_co->save_size > 0)
      memcpy(update_pending_co->stack_sp, update_pending_co->save_buffer,
             update_pending_co->save_size);
  }
}


// ------------- stack memory -------------
stack_mem_t *StackMemAlloc(unsigned int stack_size) {
  stack_mem_t *stack_mem = (stack_mem_t *)malloc(sizeof(stack_mem_t));
  stack_mem->occupy_co = nullptr;
  stack_mem->stack_size = stack_size;
  stack_mem->stack_buffer = (char *)malloc(stack_size);
  stack_mem->stack_bp = stack_mem->stack_buffer + stack_size;
  return stack_mem;
}

stack_mem_t *StackMemGet(share_stack_t *share_stack) {
  if (!share_stack) return nullptr;
  int idx = share_stack->alloc_idx % share_stack->count;
  share_stack->alloc_idx++;
  return share_stack->stack_array[idx];
}

share_stack_t *ShareStackAlloc(int count, int stack_size) {
  share_stack_t *share_stack = (share_stack_t *)calloc(1, sizeof(share_stack_t));
  share_stack->alloc_idx = 0;
  share_stack->stack_size = stack_size;
  share_stack->count = count;
  stack_mem_t **stack_array = (stack_mem_t **)calloc(count, sizeof(stack_mem_t *));
  for (int i = 0; i < count; ++i)
    stack_array[i] = StackMemAlloc(stack_size);
  share_stack->stack_array = stack_array;
  return share_stack;
}

void StackBufferSave(coroutine_t *co) {
  stack_mem_t *stack_mem = co->stack_mem;
  int len = stack_mem->stack_bp - co->stack_sp;
  if (co->save_buffer) {
    free(co->save_buffer);
    co->save_buffer = nullptr;
  }
  co->save_buffer = (char *)calloc(1, len);
  co->save_size = len;
  memcpy(co->save_buffer, co->stack_sp, len);
}

// ------------- timer -------------
timing_wheel_t *TimerAlloc(int size) {
  timing_wheel_t *timer = (timing_wheel_t *)calloc(1, sizeof(timing_wheel_t));
  timer->slot_size = size;
  timer->link = (timeout_item_link_t *)calloc(1, size * sizeof(timeout_item_link_t));
  timer->start = TickMSGet();
  timer->start_idx = 0;
  
  return timer;
}

void TimerFree(timing_wheel_t *timer) {
  if (timer) {
    free(timer->link);
    free(timer);
  }
}

int TimerItemAdd(timing_wheel_t *timer, timeout_item_t *item, unsigned long long now) {
  if (timer->start == 0) {
    timer->start = now;
    timer->start_idx = 0;
  }
  if (now < timer->start) return __LINE__;
  if (item->expire_time < now) return __LINE__;
  unsigned long long diff = item->expire_time - timer->start;
  if (diff >= (unsigned long long)timer->slot_size)
    diff = timer->slot_size - 1;
  TimeoutLinkTailAdd(timer->link + (timer->start_idx + diff) % timer->slot_size, item);
  return 0;
}

// ------------- epoll -------------
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
co_epoll_t *EpollAlloc() {
  co_epoll_t *epoll = (co_epoll_t *)calloc(1, sizeof(co_epoll_t));
  epoll->epoll_fd = epoll_create(co_epoll_t::EPOLL_SIZE);
  epoll->timer = TimerAlloc(60000); // 1 min
  epoll->active_list = (timeout_item_link_t *)calloc(1, sizeof(timeout_item_link_t));
  epoll->timeout_list = (timeout_item_link_t *)calloc(1, sizeof(timeout_item_link_t));
  return epoll;
}

void EpollFree(co_epoll_t *epoll) {
  if (epoll) {
    free(epoll->active_list);
    free(epoll->timeout_list);
    TimerFree(epoll->timer);
    CoEpollResFree(epoll->result);
    free(epoll);
  }
}

// ------------- poll -------------
// struct pollfd {
//   int   fd;         /* file descriptor */
//   short events;     /* requested events */
//   short revents;    /* returned events */
// };
int CoPoll(pollfd fds[], nfds_t nfds, int timeout, poll_pfn pollfunc) {
  if (timeout == 0) return pollfunc(fds, nfds, timeout);
  if (timeout < 0) timeout = INT_MAX;
  co_epoll_t *epoll = CurrThreadEnvGet()->epoll;
  int epfd = epoll->epoll_fd;
  coroutine_t *self = CoSelf();
  
  co_poll_t &item = *((co_poll_t *)calloc(1, sizeof(co_poll_t)));
  item.epoll_fd = epfd;
  item.fds = (pollfd *)calloc(nfds, sizeof(pollfd));
  item.nfds = nfds;
  co_poll_item_t poll_items[2];
  if (nfds < sizeof(poll_items) / sizeof(poll_items[0]) && !self->is_share_stack)
    item.poll_items = poll_items;
  else
    item.poll_items = (co_poll_item_t *)calloc(1, nfds * sizeof(co_poll_item_t));
  item.process_pfn = OnProcessEvent;
  item.args = CoSelf();

  for (nfds_t i = 0; i < nfds; ++i) {
    item.poll_items[i].self = item.fds + i;
    item.poll_items[i].poll = &item;
    item.poll_items[i].prepare_pfn  = OnPollPrepare;
    epoll_event &ev = item.poll_items[i].event;
    if (fds[i].fd > -1) {
      ev.data.ptr = item.poll_items + i;
      ev.events = PollEvent2Epoll(fds[i].events);
      int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fds[i].fd, &ev);
      if (ret < 0 && errno == EPERM && nfds == 1 && pollfunc != nullptr) {
        if (item.poll_items != poll_items) {
          free(item.poll_items);
          item.poll_items = nullptr;
        }
        free(item.fds);
        free(&item);
        return pollfunc(fds, nfds, timeout);
      }
    }
  }

  unsigned long long now = TickMSGet();
  item.expire_time = now + timeout;
  int ret = TimerItemAdd(epoll->timer, &item, now);
  int raise_cnt = 0;
  if (ret) {
    errno = EINVAL;
    raise_cnt = -1;
  } else {
    CoYield();
    raise_cnt = item.raise_cnt;
  }

  {
    TimeoutRemoveFromLink(&item);
    for (nfds_t i = 0; i < nfds; ++i) {
      int fd = fds[i].fd;
      if (fd > -1)
        epoll_ctl(epfd, EPOLL_CTL_DEL, fd, &item.poll_items[i].event);
      fds[i].revents = item.fds[i].revents;
    }
    if (item.poll_items != poll_items) {
      free(item.poll_items);
      item.poll_items = nullptr;
    }
    free(item.fds);
    free(&item);
  }
  
  return raise_cnt;
}

void OnProcessEvent(timeout_item_t *item) {
  coroutine_t *co = (coroutine_t *)item->args;
  CoResume(co);
}

// called in CoEventloop()
void OnPollPrepare(timeout_item_t *timeout_item, epoll_event &event,
                   timeout_item_link_t *active) {
  co_poll_item_t *poll_item = (co_poll_item_t *)timeout_item;
  poll_item->self->revents = EpollEvent2Poll(event.events);
  co_poll_t *item = poll_item->poll;
  item->raise_cnt++;
  if (!item->all_event_detach) {
    item->all_event_detach = 1;
    TimeoutRemoveFromLink(item);
    TimeoutLinkTailAdd(active, item);
  }
}


// ------------- condition variable -------------
int CoCondSignal(co_cond_t *cond) {
  co_cond_item_t *item = cond->head;
  if (!item) return 0;
  HeadPop<co_cond_t, co_cond_item_t>(cond);
  RemoveFromLink<timeout_item_link_t, timeout_item_t>(&item->timeout);
  TailAdd<timeout_item_link_t, timeout_item_t>(CurrThreadEnvGet()->epoll->active_list,
                                               &item->timeout);
  return 0;
}

int CoCondBroadcast(co_cond_t *cond) {
  while (1) {
    co_cond_item_t *item = cond->head;
    if (!item) return 0;
    HeadPop<co_cond_t, co_cond_item_t>(cond);
    RemoveFromLink<timeout_item_link_t, timeout_item_t>(&item->timeout);
    TailAdd<timeout_item_link_t, timeout_item_t>(CurrThreadEnvGet()->epoll->active_list,
                                                 &item->timeout);
  }
  return 0;
}

int CoCondTimewait(co_cond_t *cond, int timeout) {
  co_cond_item_t *item = (co_cond_item_t *)calloc(1, sizeof(co_cond_item_t));
  item->timeout.args = CoSelf();
  item->timeout.process_pfn = OnProcessEvent;
  if (timeout > 0) {
    unsigned long long now = TickMSGet();
    item->timeout.expire_time = now + timeout;
    int ret = TimerItemAdd(CurrThreadEnvGet()->epoll->timer, &item->timeout, now);
    if (ret) {
      free(item);
      return ret;
    }
  }
  TailAdd<co_cond_t, co_cond_item_t>(cond, item);
  CoYield();
  RemoveFromLink<co_cond_t, co_cond_item_t>(item);
  free(item);
  return 0;
}
