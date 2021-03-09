#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstdlib>
#include <cstdio>
#include <cerrno>
#include <cstring>

#include <iostream>

#define ERR_EXIT(m)				\
  do						\
    {						\
      perror(m);				\
      exit(EXIT_FAILURE);			\
    } while(0)


int main(int argc, char *argv[])
{
  int sock;
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    ERR_EXIT("socket");

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(5188);
  serverAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

  if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    ERR_EXIT("connect");

  struct sockaddr_in localAddr;
  socklen_t addrLength = sizeof(localAddr);
  if (getsockname(sock, (sockaddr*)&localAddr, &addrLength) < 0)
    ERR_EXIT("getsockname");

  std::cout << "ip = " << inet_ntoa(localAddr.sin_addr)
	    << "port = " << ntohs(localAddr.sin_port) << std::endl;

  char sendBuffer[1024] = {0};
  char recvBuffer[1024] = {0};
  while (fgets(sendBuffer, sizeof(sendBuffer), stdin) != NULL)
    {
      write(sock, sendBuffer, strlen(sendBuffer));
      read(sock, recvBuffer, sizeof(recvBuffer));

      fputs(recvBuffer, stdout);
      memset(sendBuffer, 0, sizeof(sendBuffer));
      memset(recvBuffer, 0, sizeof(recvBuffer));
    }

  close(sock);
  
  return 0;
}
