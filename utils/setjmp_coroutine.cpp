#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>

#define resume() if (setjmp(g_ctx.main_buf_) == 0) longjmp(g_ctx.co_buf_, 1);
#define yield() if (setjmp(g_ctx.co_buf_) == 0) longjmp(g_ctx.main_buf_, 1);

struct Context {
  jmp_buf main_buf_;
  jmp_buf co_buf_;
};

Context g_ctx;

void CoFunc(void *arg) {
  static int i = 0;
  while (1) {
    printf("coroutine working %d\n", i++);
    sleep(1);
    printf("corouitne suspend\n");
    yield();
  }
}

int StartCo(void (*func)(void *), void *arg) {
  if (setjmp(g_ctx.main_buf_) == 0) {
    func(arg);
    return 1;
  }
  return 0;
}

int main(int argc, char *argv[])
{
  int n = StartCo(CoFunc, nullptr);
  printf("%d\n", n);
  while (1) {
    printf("main working\n");
    sleep(1);
    printf("main suspend\n");
    resume();
  }
  return 0;
}
