#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include "co_routine.h"

struct Task{
  int id;
};

struct Context{
  co_cond_t *cond;
  std::queue<Task *> task_queue;
};

void *Producer(void *args) {
  CoEnableHook();
  Context *context = (Context *)args;
  int id = 0;
  while (1) {
    Task *task = (Task *)calloc(1, sizeof(Task));
    task->id = id++;
    context->task_queue.push(task);
    printf("produce task %d\n", task->id);
    CoCondSignal(context->cond);
    poll(nullptr, 0, 1000);
  }
  return nullptr;
}

void *Consumer(void *args) {
  CoEnableHook();
  Context *context = (Context *)args;
  while (1) {
    if (context->task_queue.empty()) {
      CoCondTimewait(context->cond, -1);
      continue;
    }
    Task *task = context->task_queue.front();
    context->task_queue.pop();
    printf("consume task %d\n", task->id);
    free(task);
  }
  return nullptr;
}

int main(int argc, char *argv[])
{
  Context *context = new Context;
  context->cond = (co_cond_t *)calloc(1, sizeof(co_cond_t));
  coroutine_t *consumer_routine = nullptr;
  CoCreate(&consumer_routine, nullptr, Consumer, context);
  CoResume(consumer_routine);
  coroutine_t *producer_routine = nullptr;
  CoCreate(&producer_routine, nullptr, Producer, context);
  CoResume(producer_routine);
  CoEventloop(nullptr, nullptr);
  return 0;
}
