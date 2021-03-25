#include <iostream>
#include <functional>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

static std::function<void()> safeQuit;

void signalintHandler(int sig)
{
  int olderrno = errno;
  if (safeQuit)
    {
      safeQuit();
      safeQuit = nullptr;
      exit(0);
    }
  errno = olderrno;
}

void signalchldHandler(int sig)
{
  int olderrno = errno;
  while (waitpid(-1, NULL, 0) > 0)
    {
      auto s = "Handler reaped child\n";
      write(STDOUT_FILENO, s, strlen(s));
    }
  if (errno != ECHILD)
    {
      auto e = "waitpid error";
      write(STDOUT_FILENO, e, strlen(e));
    }
  sleep(1);
  errno = olderrno;
}

int main(int argc, char *argv[])
{
  signal(SIGINT, signalintHandler);
  signal(SIGCHLD, signalchldHandler);
  
  // not async-signal-safety
  // safeQuit = [](){ std::cout << "do something" << std::endl; };

  // async-signal-safety: *write* is the only async-signal-safe output function
  safeQuit = []()
  {
    auto s = "Parent processing safe quit\n";
    write(STDOUT_FILENO, s, strlen(s));	
  };

  for (int i = 0; i < 3; ++i)
    {
      if (fork() == 0)
	{
	  printf("Hello from child %d\n", (int)getpid());
	  exit(0);
	}
    }
  
  char buf[10];
  read(STDIN_FILENO, buf, sizeof(buf));
  printf("Parent processing input\n");
  
  while(1)
    ;  
  return 0;
}
