#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <thread>
#include "helper.h"

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

string dir;

int resolveRequest( int clientSockfd)
{

  bool isEnd = false;
  char buf[1024] = {0};
  stringstream ss;

  int timeout = 10;
  int timer = 0;
  HttpRequest message_content;
  string content;
  string past;
  while (!isEnd)
    {
      memset(buf, '\0', sizeof(buf));
      while (recv(clientSockfd, buf, 1024, 0) == -1)
        {
          timer++;
          sleep(1);
          if(timer >= timeout)
            {
	      close(clientSockfd); //close connection
            }
        }

      ss << buf << endl;

      string tmp = past + ss.str();
      content += ss.str();

      if(tmp.find("\r\n\r\n") == string::npos)
        {
          past = ss.str();
          ss.str("");
          continue;
	}
      else   //we did find the complete string                                             
        {
          isEnd= true;
        }
    }

  message_content.decode(content);
  HttpResponse response;
  if(!message_content.isValid())
    {
      response.setStatus("400 BAD REQUEST");
      string r = response.encode();

      if (write(clientSockfd, r.c_str(), r.size()) == -1)
        {
          perror("write");
          return 4;
        }

      close(clientSockfd);
    }

  ifstream myReadFile;
  string fileName;

  if(dir == ".")
    {
      fileName = message_content.getPath();
      fileName = fileName.substr(1,fileName.length()-1);
    }
  else
    {
      fileName = dir + "/" + message_content.getPath();
    }
 
  myReadFile.open(fileName, ios::binary);
  if(myReadFile.is_open())
    {
      // file successfully found 
      response.setStatus("200 OK");
      string tmp;
      unsigned char i;

      while(1) {
        myReadFile.read((char *)&i, sizeof(i));
        if(myReadFile.eof()) 
	  break;
        tmp += i;
      }
      response.setBody(tmp);
      response.setBodySize(tmp.size());
      myReadFile.close();
    }
  else if(myReadFile.fail())
    {
      response.setStatus("404 NOT FOUND");
      response.setBodySize(0);
    }

  response.setResponse(response.getStatus(),response.getBody());
  string r = response.encode();

  if (write(clientSockfd, r.c_str(), r.size()) == -1)
    {
      perror("write");
      return 4;
    }

  //send the response to the client  
  close(clientSockfd);
  return 0;
}



int main(int argc, char *argv[])
{
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  string host_name;
  string port_number;

  //check argument numbers
  if(argc == 2 || argc == 3 || argc > 4)
    {
      cerr << "ERROR: Invalid number of arguments";
    }
  else if(argc == 1)
    {
      host_name = "localhost";
      port_number = "4000";
      dir = ".";
    }
  else
    {
      host_name = argv[1];
      port_number = argv[2];
      dir = argv[3];
    }
  
  // allow others to reuse the address
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    perror("setsockopt");
    return 1;
  }
  
  // bind address to socket
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(atoi(port_number.c_str()));     // short, network byte order
  addr.sin_addr.s_addr = inet_addr(getIP(host_name, port_number).c_str());
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if (::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 2;
  }

  // set socket to listen status                                                       
  if (listen(sockfd, 1) == -1) {
    perror("listen");
    return 3;
  }
  
  thread *thread_ptr;
  while(1)
    {
      // accept a new connection
      struct sockaddr_in clientAddr;
      socklen_t clientAddrSize = sizeof(clientAddr);
      int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

      if (clientSockfd == -1) {
	perror("accept");
	return 4;
      }

      char ipstr[INET_ADDRSTRLEN] = {'\0'};
      inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
      std::cout << "Accept a connection from: " << ipstr << ":" <<
	ntohs(clientAddr.sin_port) << std::endl;

      thread_ptr = new thread (resolveRequest, clientSockfd);
      thread_ptr->detach();

      //every time a connection is made, that connection will get its own thread
    }
}
