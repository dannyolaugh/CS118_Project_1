#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include "header.hpp"

int main(int argc, char* argv[])
{
  char* hostname;
  char* portNum;
  char* dir;
  char* ipstr[INET_ADDRSTRLEN] = {'\0'};

  if(argc == 2 || argc == 3 || argc > 4)
    {
      std::cerr << "Invalid Argument Number" <<endl;
      return 0
    }
  else if( argc == 1)
    {
      ipstr = "127.0.0.1";
      portNum = "4000";
      dir = ".";
    }
  else
    {
      hostname = argv[1];
      portNum = argv[2];
      dir = argv[3];
      
      struct addrinfo hints;
      struct addrinfo* res;
      
      // prepare hints
      memset(&hints, 0, sizeof(hints));
      hints.ai_family = AF_INET; // IPv4
      hints.ai_socktype = SOCK_STREAM; // TCP
      
      // get address
      int status = 0;
      if ((status = getaddrinfo(hostname, portNum, &hints, &res)) != 0) {
	std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
	return 2;
      }
      
      for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
	// convert address to IPv4 address
	struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
	
	// convert the IP to a string and print it:
	inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));  
      }
      free addrinfo(res); // free the linked list
      
    }
  
  
  
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  // allow others to reuse the address
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }

  // bind address to socket
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(40000);     // short, network byte order
  addr.sin_addr.s_addr = inet_addr(ipstr);
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 2;
  }

  // set socket to listen status
  if (listen(sockfd, 1) == -1) {
    perror("listen");
    return 3;
  }

  // accept a new connection
  struct sockaddr_in clientAddr;
  socklen_t clientAddrSize = sizeof(clientAddr);
  int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

  if (clientSockfd == -1) {
    perror("accept");
    return 4;
  }

  // read/write data from/into the connection
  bool isEnd = false;
  string buf;

  while (!isEnd) {
    
    if (read(clientSockfd, buf, sizeof[buf]) == -1) {
      perror("read");
      return 5;
    }

    //search
    

    if (write(clientSockfd, buf, 20, 0) == -1) {
      perror("send");
      return 6;
    }

    if (ss.str() == "close\n")
      break;

    ss.str("");
  }

  close(clientSockfd);

  return 0;
}
