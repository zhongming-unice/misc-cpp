#include <queue>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#include "co_routine.h"

void SetAndGetEnv(int routine_id) {
  printf("routine %d begin\n", routine_id);
  poll(nullptr, 0, 500);
  char buf[128];
  sprintf(buf, "routine_%d", routine_id);
  int ret = setenv("CGINAME", buf, 1);
  if (ret) {
    printf("%s:%d set env error, errno: %d %s\n", __func__, __LINE__,
           errno, strerror(errno));
    return;
  }
  printf("routine %d set env CGINAME %s\n", routine_id, buf);
  poll(nullptr, 0, 500);
  char *env = getenv("CGINAME");
  printf("routine %d get env CGINAME %s\n", routine_id, env);
}

void *RoutineFunc(void *args) {
  CoEnableHook();
  SetAndGetEnv(*((int *)args));
  return nullptr;
}
int main(int argc, char *argv[])
{
  const char* ENV_NAME[] = {"CGINAME"};
  SetEnvList(ENV_NAME, 1);
  for (int i = 0; i < 3; ++i) {
    int id = i;
    coroutine_t *co = nullptr;
    CoCreate(&co, nullptr, RoutineFunc, &id);
    CoResume(co);
  }
  CoEventloop(nullptr, nullptr);
  return 0;
}
