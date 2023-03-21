#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#include <vector>

#include "blocking_queue.h"


void *producer_func(void *arg)
{
  BlockingQueue<int> *q = (BlockingQueue<int> *)arg;
  int goods = 1;
  while (1) {
    sleep(1);
    q->put(goods++);
  }
}

void *consumer_func(void *arg)
{
  BlockingQueue<int> *q = (BlockingQueue<int> *)arg;
  while (1) {
    int goods;
    if (q->take(goods))
      printf("%d - %d thread get %d\n", getpid(), gettid(), goods);
  }
}

void *consumer_func_2(void *arg)
{
  BlockingQueue<int> *q = (BlockingQueue<int> *)arg;
  while (1) {
    int goods = q->take();
    printf("%d - %d thread get %d\n", getpid(), gettid(), goods);
  }
}

int main(int argc, char *argv[])
{
  BlockingQueue<int> queue;
  std::vector<pthread_t> threads(12);
  for (int i = 0; i < 2; ++i) {
    pthread_create(&threads[i], NULL, producer_func, &queue);
  }
  for (int i = 2; i < 7; ++i) {
    pthread_create(&threads[i], NULL, consumer_func, &queue);
  }
  for (int i = 7; i < threads.size(); ++i) {
    pthread_create(&threads[i], NULL, consumer_func_2, &queue);
  }
  for (int i = 0; i < threads.size(); ++i) {
    pthread_join(threads[i], NULL);  
  }
  
  return 0;
}
