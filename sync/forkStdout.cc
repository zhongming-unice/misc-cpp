#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
/*
 * write: print 6 : (6 = 2 ^ 3 - 2)
 * printf: print 8  
 * std::cout: print 8 
 * printf/std::cout + fflush/std::flush: print 6 
 */
int main(void)
{
   int i;
   for (i = 0; i < 2; i++)
     {
       auto pid = fork();
       // write(STDIN_FILENO, "#", 1);
       // printf("-");
       // fflush(stdout);
       std::cout << "#" << pid;
       // std::cout << std::flush;
     }

   wait(NULL);
   wait(NULL);
   
   return 0;
}
