// a simple implementation of ls | wc -l

#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int fd[2];
  pipe(fd);
  pid_t pid = fork();

  if (pid == 0)
    {
      close(fd[1]);		// close write pipe
      dup2(fd[0], STDIN_FILENO);
      execlp("wc", "wc", "-l", NULL);
    }
  else
    {
      close(fd[0]);		// 
      dup2(fd[1], STDOUT_FILENO);
      execlp("ls", "ls", NULL);
    }
  
  // int status;
  // waitpid(pid, &status, 0);
  
  return 0;
}
