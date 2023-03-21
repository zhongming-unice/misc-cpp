#include <sys/socket.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/un.h>
#include <dlfcn.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <errno.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <pthread.h>
#include <resolv.h>
#include <netdb.h>
#include <time.h>

#include <map>

#include "co_routine.h"

// ------------- poll -------------
typedef int (*poll_pfn)(pollfd fds[], nfds_t nfds, int timeout);
static poll_pfn sys_poll_func = (poll_pfn)dlsym(RTLD_NEXT, "poll");

int poll(pollfd fds[], nfds_t nfds, int timeout) {
  if (!CoSelf()->enable_sys_hook || timeout == 0)
    return sys_poll_func(fds, nfds, timeout);
  pollfd *fds_merge = nullptr;
  nfds_t nfds_merge = 0;
  std::map<int, int> m;         // fd --> idx
  std::map<int, int>::iterator it;
  if (nfds > 1) {
    fds_merge = (pollfd *)calloc(1, sizeof(pollfd) * nfds);
    for (size_t i = 0; i < nfds; ++i) {
      it = m.find(fds[i].fd);
      if (it == m.end()) {
        fds_merge[nfds_merge] = fds[i];
        m[fds[i].fd] = nfds_merge;
        nfds_merge++;
      } else {
        fds_merge[it->second].events |= fds[i].events;
      }
    }
  }

  int ret = 0;
  if (nfds_merge == nfds || nfds == 1)
    ret = CoPoll(fds, nfds, timeout, sys_poll_func);
  else {
    ret = CoPoll(fds_merge, nfds_merge, timeout, sys_poll_func);
    if (ret > 0) {
      for (size_t i = 0; i < nfds; ++i) {
        it = m.find(fds[i].fd);
        if (it != m.end())
          fds[i].revents = fds_merge[it->second].revents & fds[i].events;
      }
    }
  }
  free(fds_merge);
  return ret;
}

// ------------- env -------------
struct env_t {
  char *name;
  char *value;
};

struct env_list_t {
  env_t *data;
  size_t count;
};

static env_list_t g_env_list = {0};

static int EnvComp(const void *lhs, const void *rhs) {
  return strcmp(((env_t *)lhs)->name, ((env_t *)rhs)->name);
}

void SetEnvList(const char *name[], size_t count) {
  if (g_env_list.data) return;
  g_env_list.data = (env_t *)calloc(1, sizeof(env_t) * count);
  for (size_t i = 0; i < count; ++i)
    if (name[i] && name[i][0])
      g_env_list.data[g_env_list.count++].name = strdup(name[i]);
  if (g_env_list.count > 1) {
    qsort(g_env_list.data, g_env_list.count, sizeof(env_t), EnvComp);
  }
}

static env_list_t *DupEnvList(env_list_t *env_list) {
  env_list_t *ret = (env_list_t *)calloc(1, sizeof(env_list_t));
  if (env_list->count) {
    ret->data = (env_t *)calloc(1, sizeof(env_t) * env_list->count);
    ret->count = env_list->count;
    memcpy(ret->data, env_list->data, sizeof(env_t) * env_list->count);
  }
  return ret;
}

typedef int (*setenv_pfn)(const char *name, const char *value, int overwrite);
typedef int (*unsetenv_pfn)(const char *name);
typedef char *(*getenv_pfn)(const char *name);
static setenv_pfn sys_setenv_func = (setenv_pfn)dlsym(RTLD_NEXT, "setenv");
static unsetenv_pfn sys_unsetenv_func = (unsetenv_pfn)dlsym(RTLD_NEXT, "unsetenv");
static getenv_pfn sys_getenv_func = (getenv_pfn)dlsym(RTLD_NEXT, "getenv");

int setenv(const char *name, const char *value, int overwrite) {
  coroutine_t *self = CoSelf();
  if (self->enable_sys_hook && g_env_list.data) {
    if (!self->pv_env)
      self->pv_env = DupEnvList(&g_env_list);
    env_list_t *env_list = (env_list_t *)(self->pv_env);
    env_t env = {(char *)name, 0};
    env_t *e = (env_t *)bsearch(&env, env_list->data,
                                env_list->count, sizeof(env_t), EnvComp);
    if (e) {
      if (overwrite || !e->value) {
        if (e->value) free(e->value);
        e->value = (value ? strdup(value) : 0);
      }
      return 0;
    }
    
  }
  return sys_setenv_func(name, value, overwrite);
}

int unsetenv(const char *name) {
  coroutine_t *self = CoSelf();
  if (self->enable_sys_hook && g_env_list.data) {
    if (!self->pv_env)
      self->pv_env = DupEnvList(&g_env_list);
    env_list_t *env_list = (env_list_t *)(self->pv_env);
    env_t env = {(char *)name, 0};
    env_t *e = (env_t *)bsearch(&env, env_list->data,
                                env_list->count, sizeof(env_t), EnvComp);
    if (e) {
      if (e->value) {
        free(e->value);
        e->value = 0;
      }
      return 0;
    }
    
  }
  return sys_unsetenv_func(name);
}

char *getenv(const char *name) {
  coroutine_t *self = CoSelf();
  if (self->enable_sys_hook && g_env_list.data) {
    if (!self->pv_env)
      self->pv_env = DupEnvList(&g_env_list);
    env_list_t *env_list = (env_list_t *)(self->pv_env);
    env_t env = {(char *)name, 0};
    env_t *e = (env_t *)bsearch(&env, env_list->data,
                                env_list->count, sizeof(env_t), EnvComp);
    if (e) return e->value;
  }
  return sys_getenv_func(name);
}

// ------------- socket -------------
// in <sys/socket.h>
// struct sockaddr {
//   u_short sa_family;            /* address family;  AF_xxx */
//   char sa_data[14];             /* protocol specific address */
// };

// in <sys/un.h>
// struct sockaddr_un{
//   short sun_family;             /*AF_UNIX*/
//   char sun_PATH[108];           /*path name */
// };

// in <netinet/in.h>
// struct in_addr {
//   u_long s_addr;                /*32-bit net id */
// };
// struct sockaddr_in {
//   short sin_family;                 /* AF_INET */
//   u_short sin_port;                 /* 16-bit port number htons() */
//   struct in_addr sin_addr;          /* htonl(INADDR_ANY) */
//   char sin_zero[8];                 /* unused */
// };

// const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);

struct socket_info {
  int flag;
  sockaddr dest;
  int domain;
  timeval read_timeout;
  timeval write_timeout;
};

static socket_info *g_socket_info[102400] = {0};
static inline socket_info *SocketInfoAlloc(int fd) {
  if (fd > -1 && fd < 102400) {
    socket_info *info = (socket_info *)calloc(1, sizeof(socket_info));
    info->read_timeout.tv_sec = 1;
    info->write_timeout.tv_sec = 1;
    g_socket_info[fd] = info;
    return info;
  }
  return nullptr;
}
static inline socket_info *SocketInfoGet(int fd) {
  if (fd > -1 && fd < 102400)
    return g_socket_info[fd];
  return nullptr;
}
static inline void SocketInfoFree(int fd) {
  if (fd > -1 && fd < 102400) {
    socket_info *info = g_socket_info[fd];
    if (info) {
      free(info);
      g_socket_info[fd] = nullptr;
    }
  }
  return;
}

// --------------------------------------------------
typedef int (*fcntl_pfn)(int fd, int cmd, ... /* arg */ );
static fcntl_pfn sys_fcntl_func = (fcntl_pfn)dlsym(RTLD_NEXT, "fcntl");

int fcntl(int fd, int cmd, ...) {
  if (fd < 0) return __LINE__;
  va_list arg_list;
  va_start(arg_list, cmd);
  int ret = -1;
  socket_info *info = SocketInfoGet(fd);
  switch (cmd) {
  case F_DUPFD:
  case F_DUPFD_CLOEXEC:
  case F_SETFD:
  case F_SETOWN: {
    int param = va_arg(arg_list, int);
    ret = sys_fcntl_func(fd, cmd, param);
    break;
  }
  case F_GETFD:
  case F_GETOWN: {
    ret = sys_fcntl_func(fd, cmd);
    break;
  }
  case F_GETFL: {
    ret = sys_fcntl_func(fd, cmd);
    if (info && !info->flag & O_NONBLOCK)
      ret = ret & (~O_NONBLOCK);
    break;
  }
  case F_SETFL: {
    int param = va_arg(arg_list, int);
    if (CoSelf()->enable_sys_hook && info) param |= O_NONBLOCK;
    ret = sys_fcntl_func(fd, cmd, param);
    if (info && ret == 0) info->flag = param;
    break;
  }
  case F_GETLK:
  case F_SETLK:
  case F_SETLKW: {
    flock *param = va_arg(arg_list, flock *);
    ret = sys_fcntl_func(fd, cmd, param);
    break;
  }
  }
  va_end(arg_list);
  return ret;
}

// --------------------------------------------------
typedef int (*socket_pfn)(int domain, int type, int protocol);
static socket_pfn sys_socket_func = (socket_pfn)dlsym(RTLD_NEXT, "socket");

/*
 * @param domain: specifies communication domain(pf unix or pf internet)
 * @param type: specifies communication semantics(stream) and socket type(non block, cloexec)
 * @param protocol: specifies particular protocol
 * @return fd: file descriptor for the new socket
 */
int socket(int domain, int type, int protocol) {
  coroutine_t *self = CoSelf();
  int fd = sys_socket_func(domain, type, protocol);
  if (!self->enable_sys_hook || fd < 0) return fd;
  socket_info *info = SocketInfoAlloc(fd);
  info->domain = domain;
  // F_SETFL: Set the file status flags to the value specified by arg
  // F_GETFL: Get the file access mode and the file status flags
  fcntl(fd, F_SETFL, sys_fcntl_func(fd, F_GETFL, 0));
  return fd;
}

// --------------------------------------------------
typedef int (*setsockopt_pfn)(int sockfd, int level, int optname,
                              const void *optval, socklen_t optlen);
static setsockopt_pfn sys_setsockopt_func = (setsockopt_pfn)dlsym(RTLD_NEXT, "setsockopt");

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
  if (!CoSelf()->enable_sys_hook)
    return sys_setsockopt_func(sockfd, level, optname, optval, optlen);
  socket_info *info = SocketInfoGet(sockfd);
  if (info && level == SOL_SOCKET) {
    timeval *tv = (timeval *)optval;
    if (optname == SO_RCVTIMEO)
      memcpy(&info->read_timeout, tv, sizeof(*tv));
    else if (optname == SO_SNDTIMEO)
      memcpy(&info->write_timeout, tv, sizeof(*tv));
  }
  return sys_setsockopt_func(sockfd, level, optname, optval, optlen);
}

// --------------------------------------------------
typedef int (*bind_pfn)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
static bind_pfn sys_bind_func = (bind_pfn)dlsym(RTLD_NEXT, "bind");

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  if (!CoSelf()->enable_sys_hook) return sys_bind_func(sockfd, addr, addrlen);
  socket_info *info = SocketInfoGet(sockfd);
  if (info)
    memcpy(&info->dest, addr, addrlen);
  return sys_bind_func(sockfd, addr, addrlen);
}

// --------------------------------------------------
typedef int (*listen_pfn)(int sockfd, int backlog);
static listen_pfn sys_listen_func = (listen_pfn)dlsym(RTLD_NEXT, "listen");

int listen(int sockfd, int backlog) {
  return sys_listen_func(sockfd, backlog);
}

// --------------------------------------------------
typedef int (*accept4_pfn)(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags);
static accept4_pfn sys_accept4_func = (accept4_pfn)dlsym(RTLD_NEXT, "accept4");

int accept4(int sockfd, struct sockaddr *addr, socklen_t *addrlen, int flags) {
  int accepted_socket = sys_accept4_func(sockfd, addr, addrlen, flags);
  if (CoSelf()->enable_sys_hook && accepted_socket >= 0)
    SocketInfoAlloc(accepted_socket);
  return accepted_socket;
}

// --------------------------------------------------
typedef int (*close_pfn)(int fd);
static close_pfn sys_close_func = (close_pfn)dlsym(RTLD_NEXT, "close");

int close(int fd) {
  if (!CoSelf()->enable_sys_hook) return sys_close_func(fd);
  SocketInfoFree(fd);
  return sys_close_func(fd);
}

// --------------------------------------------------
typedef int (*connect_pfn)(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
static connect_pfn sys_connect_func = (connect_pfn)dlsym(RTLD_NEXT, "connect");

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
  int ret = sys_connect_func(sockfd, addr, addrlen); 
  if (!CoSelf()->enable_sys_hook)
    return ret;
  socket_info *info = SocketInfoGet(sockfd);
  if (!info) return ret;
  memcpy(&info->dest, addr, addrlen);
  if (info->flag & O_NONBLOCK) return ret;
  if (!(ret < 0 && errno == EINPROGRESS)) return ret;
  int poll_ret = 0;
  pollfd poll_fd = {0};
  memset(&poll_fd, 0, sizeof(poll_fd));
  poll_fd.fd = sockfd;
  poll_fd.events = POLLOUT | POLLERR | POLLHUP;
  while (1) {
    poll_ret = poll(&poll_fd, 1, 10000);
    if (poll_ret == 1) break;
  }
  // connect succ
  if  (poll_fd.revents & POLLOUT) {
    int err = 0;
    socklen_t errlen = sizeof(err);
    ret = getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &errlen);
    if (ret < 0) return ret;
    if (err != 0) {
      errno = err;
      return -1;
    }
    errno = 0;
    return 0;
  }
  errno = ETIMEDOUT;
  return ret;
}

// --------------------------------------------------
typedef ssize_t (*read_pfn)(int fd, void *buf, size_t count);
static read_pfn sys_read_func = (read_pfn)dlsym(RTLD_NEXT, "read");

ssize_t read(int fd, void *buf, size_t count) {
  if (!CoSelf()->enable_sys_hook) return sys_read_func(fd, buf, count);
  socket_info *info = SocketInfoGet(fd);
  if (!info || info->flag & O_NONBLOCK)
    return sys_read_func(fd, buf, count);
  int timeout = info->read_timeout.tv_sec * 1000 + info->read_timeout.tv_usec / 1000;
  pollfd poll_fd = {0};
  poll_fd.fd = fd;
  poll_fd.events = POLLIN | POLLERR | POLLHUP;
  poll(&poll_fd, 1, timeout);
  return sys_read_func(fd, buf, count);
}

// --------------------------------------------------
typedef ssize_t (*write_pfn)(int fd, const void *buf, size_t count);
static write_pfn sys_write_func = (write_pfn)dlsym(RTLD_NEXT, "write");

ssize_t write(int fd, const void *buf, size_t count) {
  if (!CoSelf()->enable_sys_hook) return sys_write_func(fd, buf, count);
  socket_info *info = SocketInfoGet(fd);
  if (!info || info->flag & O_NONBLOCK)
    return sys_write_func(fd, buf, count);
  int timeout = info->write_timeout.tv_sec * 1000 + info->write_timeout.tv_usec / 1000;
  ssize_t ret = sys_write_func(fd, buf, count);
  if (ret == 0) return 0;
  size_t len = 0;
  if (ret > 0) len += ret;
  while (len < count) {
    pollfd poll_fd = {0};
    poll_fd.fd = fd;
    poll_fd.events = POLLIN | POLLERR | POLLHUP;
    poll(&poll_fd, 1, timeout);
    ret = sys_write_func(fd, (const char *)buf + len, count - len);
    if (ret <= 0) break;
    len += ret;
  }
  if (ret <= 0 && len == 0) return ret;
  return len;
}

// --------------------------------------------------
typedef ssize_t (*sendto_pfn)(int sockfd, const void *buf, size_t len, int flags,
                              const struct sockaddr *dest_addr, socklen_t addrlen);
static sendto_pfn sys_sendto_func = (sendto_pfn)dlsym(RTLD_NEXT, "sendto");

ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
               const struct sockaddr *dest_addr, socklen_t addrlen) {
  ssize_t ret = sys_sendto_func(sockfd, buf, len, flags, dest_addr, addrlen);
  if (!CoSelf()->enable_sys_hook)
    return ret;
  socket_info *info = SocketInfoGet(sockfd);
  if (!info || info->flag & O_NONBLOCK)
    return ret;
  if (ret < 0 && errno == EAGAIN) {
    int timeout = info->write_timeout.tv_sec * 1000 + info->write_timeout.tv_usec / 1000;
    pollfd poll_fd = {0};
    poll_fd.fd = sockfd;
    poll_fd.events = POLLOUT | POLLERR | POLLHUP;
    poll(&poll_fd, 1, timeout);
    ret = sys_sendto_func(sockfd, buf, len, flags, dest_addr, addrlen);
  }
  return ret;
}

// --------------------------------------------------
typedef ssize_t (*recvfrom_pfn)(int sockfd, void *buf, size_t len, int flags,
                                struct sockaddr *src_addr, socklen_t *addrlen);
static recvfrom_pfn sys_recvfrom_func = (recvfrom_pfn)dlsym(RTLD_NEXT, "recvfrom");

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src_addr, socklen_t *addrlen) {
  if (!CoSelf()->enable_sys_hook)
    return sys_recvfrom_func(sockfd, buf, len ,flags, src_addr, addrlen);
  socket_info *info = SocketInfoGet(sockfd);
  if (!info || info->flag & O_NONBLOCK)
    return sys_recvfrom_func(sockfd, buf, len ,flags, src_addr, addrlen);
  int timeout = info->write_timeout.tv_sec * 1000 + info->write_timeout.tv_usec / 1000;
  pollfd poll_fd = {0};
  poll_fd.fd = sockfd;
  poll_fd.events = POLLOUT | POLLERR | POLLHUP;
  poll(&poll_fd, 1, timeout);
  return sys_recvfrom_func(sockfd, buf, len ,flags, src_addr, addrlen);
}

// --------------------------------------------------

// ------------- other -------------

void CoEnableHook() {
  CoSelf()->enable_sys_hook = 1;
}
