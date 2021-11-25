#include <pthread.h>
#include <stdio.h>
#include <asm/prctl.h>
#include <sys/prctl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

static int __thread tls1 = 0;
static int __thread tls2 = 0;

void *routine(void *) {
  unsigned long pthread_addr;
  syscall(SYS_arch_prctl, ARCH_GET_FS, &pthread_addr);
  printf("pthread addr = %p, tls1 addr = %p, tls2 addr = %p, offset1 = %ld, offset2 = %ld\n",
         pthread_addr, &tls1, &tls2, (long)&tls1 - pthread_addr, (long)&tls2 - pthread_addr);
  return nullptr;
}

int main() {
  pthread_t threads[3];
  int i = 0;
  for (i = 0; i < 3; ++i)
    pthread_create(&threads[i], nullptr, routine, nullptr);
  for (i = 0; i < 3; ++i)
    pthread_join(threads[i], nullptr);
  return 0;
}
