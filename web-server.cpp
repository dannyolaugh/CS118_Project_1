#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <sstream>
#include "helper.h"
using namespace std;

int main(int argc, char* argv[])
{
  string hostname;
  string portNum;
  string dir;
  string ipstr;

  if(argc == 2 || argc == 3 || argc > 4)
    {
      std::cerr << "Invalid Argument Number" <<endl;
      return 0;
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

      //      ipstr = getIP(hostname, portNum);
    }

  /*
  ifstream file;
  request.getPath();
  string fileName;
  if (dir != ".") {
    filename = dir + request.getPath();
    cout << "filename: " << filename << endl;
  }

  inStream.open(filename,ios::binary);

  
  // 404 file not found
  if (inStream.fail())
    {
      cout << "Requested File Not Found" << endl;
      response.setStatus("404 NOT FOUND");
      response.setBodySize(0);
      response.encode();
    }
  else
    {
      // file successfully found
      response.setStatus("200 OK");
      string tmp; 
      unsigned char i;

      while(1) {
	inStream.read((char *)&i, sizeof(i));
	if(inStream.eof()) 
	  break;
	tmp+=i;
      }


      response.setBody(tmp);  
      response.setBodySize(tmp.size());

      response.encode();
    }
  */
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
  addr.sin_addr.s_addr = inet_addr(ipstr.c_str());
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
  //  bool isEnd = false;
  //string buf;
  /*
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
  */
  close(clientSockfd);

  return 0;
}
