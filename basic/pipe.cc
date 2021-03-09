#include <iostream>
#include <unistd.h>

const int BUF_SIZE = 30;

int main(int argc, char *argv[])
{
  int fds1[2], fds2[2];
  char str1[] = "hi jojo !";
  char str2[] = "who are you ?";
  char buf[BUF_SIZE];
  pid_t pid;

  pipe(fds1), pipe(fds2);
  pid = fork();
  if (pid == 0)
    {
      write(fds1[1], str1, sizeof(str1));
      read(fds2[0], buf, BUF_SIZE);
      std::cout << "Child process output: " << buf << std::endl;
    }
  else
    {
      write(fds2[1], str2, sizeof(str2));
      read(fds1[0], buf, BUF_SIZE);
      std::cout << "Parent process output: " << buf << std::endl;
    }
  
  return 0;
}
