// a simple implementation of nslookup

#include <iostream>
#include <netdb.h>

#define	MAXLINE	 8192  /* Max text line length */

int main(int argc, char *argv[])
{
  struct addrinfo *p, *listp, hints;
  char buf[MAXLINE];
  int rc, flags;

  memset(&hints, 0, sizeof(struct addrinfo));
  // hints.ai_family = AF_INET; // only IPv4
  hints.ai_socktype = SOCK_STREAM;
  
  if ((rc = getaddrinfo(argv[1], nullptr, &hints, &listp)) != 0)
    {
      std::cerr << "getaddrinfo error: " << gai_strerror(rc);
      exit(1);
    }

  
  flags = NI_NUMERICHOST; 	// display address instead of name

  for (p = listp; p; p = p->ai_next)
    {
      if ((rc = getnameinfo(p->ai_addr, p->ai_addrlen,
			    buf, MAXLINE, nullptr, 0, flags)) != 0)
	{
	  std::cerr << "getnameinfo error: " << gai_strerror(rc);
	}
      std::cout << buf << std::endl;
    }

  freeaddrinfo(listp);
  return 0;
}
