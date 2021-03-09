/* 
 * goodcnt.c - An properly synchronized counter program 
 */
/* $begin goodcnt */
#include <pthread.h>		/* for pthread_t */
#include <semaphore.h>		/* for sem_t */
#include <stdio.h>		/* for printf,  */
#include <stdlib.h>		/* for exit, atoi */
#include <string.h>	      	/* for strerror */
#include <sys/errno.h>		/* for errno */
void *thread(void *vargp);  /* Thread routine prototype */

/* Global shared variable */
volatile long cnt = 0; /* Counter */
sem_t mutex;	       /* Semaphore that protects cnt */


void P(sem_t *sem) 
{
  if (sem_wait(sem) < 0)
    fprintf(stderr, "%s: %s\n", "P error", strerror(errno));
}

void V(sem_t *sem) 
{
  if (sem_post(sem) < 0)
    fprintf(stderr, "%s: %s\n", "V error", strerror(errno));
}

int main(int argc, char **argv) 
{
  /* if (sem_open("mutex", O_CREAT|O_EXCL, S_IRWXU, 0) < 0) /\* for mac ox *\/ */
  if (sem_init(&mutex, 0, 1) < 0)	/* mutex = 1 */
    {
      fprintf(stderr, "%s: %s\n", "sem_init error", strerror(errno));
      exit(0);
    }
  
  long niters;
  pthread_t tid1, tid2;

  /* Check input argument */
  if (argc != 2) { 
    printf("usage: %s <niters>\n", argv[0]);
    exit(0);
  }
  niters = atoi(argv[1]);

  /* Create threads and wait for them to finish */
  int rc;
  if ((rc = pthread_create(&tid1, NULL, thread, &niters)) != 0)
    {
      fprintf(stderr, "%s: %s\n", "pthread_create error", strerror(rc));
      exit(0);
    }
  if ((rc = pthread_create(&tid2, NULL, thread, &niters)) != 0)
    {
      fprintf(stderr, "%s: %s\n", "pthread_create error", strerror(rc));
      exit(0);
    }
  if ((rc = pthread_join(tid1, NULL)) != 0)
    {
      fprintf(stderr, "%s: %s\n", "pthread_join error", strerror(rc));
      exit(0);
    }
  if ((rc = pthread_join(tid2, NULL)) != 0)
    {
      fprintf(stderr, "%s: %s\n", "pthread_join error", strerror(rc));
      exit(0);
    }

  /* Check result */
  if (cnt != (2 * niters))
    printf("BOOM! cnt=%ld\n", cnt);
  else
    printf("OK cnt=%ld\n", cnt);
  exit(0);
}

/* Thread routine */
void *thread(void *vargp) 
{
  long i, niters = *((long *)vargp);

  for (i = 0; i < niters; i++)
    {
      P(&mutex);
      cnt++;
      V(&mutex);
    }

  return NULL;
}
/* $end goodcnt */
