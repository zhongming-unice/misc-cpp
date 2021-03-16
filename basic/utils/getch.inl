#ifndef _GETCH_INL_
#define _GETCH_INL_

/*
 * convert getchar() to getch()
 *
 * getchar() or std::cin().get() is buffered reading, 
 * should press enter.
 *
 * while getch() is:
 * no echoing of characters.
 * unbuffered reading of characters.
 */

#ifdef _WIN32
#include <conio.h>
#else
#ifdef __linux__
#include <termio.h>
#include <cstdio>
#elif __APPLE__
#include <termios.h>
#endif
inline int getch(void)
{
  struct termios term, term_tmp;
  int c, fd = 0;
  tcgetattr(fd, &term);
  cfmakeraw(&term_tmp);
  tcsetattr(fd, TCSANOW, &term_tmp);
  c = getchar();
  tcsetattr(fd, TCSANOW, &term);
  return c;
}

#endif

#endif
