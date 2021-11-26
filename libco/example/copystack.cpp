#include <string.h>
#include <stdio.h>
#include "co_routine.h"

void *Func(void *args) {
  CoEnableHook();
  int *id = (int *)args;
  while (1) {
    char buf[128];
    sprintf(buf, "routine %d: stack address %p\n", *id, buf);
    printf("%s", buf);
    poll(nullptr, 0, 1000);
  }
  return nullptr;
}

int main(int argc, char *argv[])
{
  share_stack_t *share_stack = ShareStackAlloc(2, 128 * 1024);
  co_attr_t attr;
  attr.stack_size = 0;
  attr.share_stack = share_stack;
  coroutine_t *co[3];
  int id[3];
  for (int i = 0; i < 3; ++i) {
    id[i] = i;
    CoCreate(&co[i], &attr, Func, id + i);
    CoResume(co[i]);
  }
  CoEventloop(nullptr, nullptr);
  return 0;
}
