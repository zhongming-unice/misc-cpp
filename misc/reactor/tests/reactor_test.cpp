#include "reactor.h"

void *net_server_thread_func(void *arg)
{
  NetServer *net_server = (NetServer *)arg;
  net_server->Run();
  return NULL;
}

int main(int argc, char *argv[])
{
  NetServer net_server;
  net_server.ListenPort(9999);
  pthread_t net_server_thread;
  pthread_attr_t net_server_attr;
  pthread_attr_init(&net_server_attr);
  pthread_attr_setdetachstate(&net_server_attr, PTHREAD_CREATE_DETACHED);
  pthread_create(&net_server_thread, &net_server_attr,
                 net_server_thread_func, (void *)&net_server);
  while (1) {
    for (auto it = net_server.read_queues_.begin();
         it != net_server.read_queues_.end(); ++it) {
      int client_fd = it->first;
      BlockingQueue<std::string> &queue = it->second;
      std::string msg;
      while (queue.take(msg)) {
        msg = std::to_string(client_fd) + "  " + msg;
        int nwrite = write(client_fd, msg.data(), msg.size());
        if (nwrite == -1) {
          net_server.write_queues_[client_fd].put(msg);
        }
      }
    }
  }
            
  return 0;
}
