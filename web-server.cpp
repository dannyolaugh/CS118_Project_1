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

int resolveRequest(sockaddr_in  clientAddr, int clientSockfd);
string dir;

int main(int argc, char *argv[])
{
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  string host_name;
  unsigned short port_number;
  
  if(argc == 2 || argc == 3 || argc > 4)
    {
      cerr << "ERROR: Invalid number of arguments";
    }
  else if(argc == 1)
    {
      host_name = "localhost";
      port_number = 4000;
      dir = ".";
    }
  else
    {
      host_name = argv[1];
      port_number = stoul(argv[2]);
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
  addr.sin_port = htons(port_number);     // short, network byte order
  addr.sin_addr.s_addr = inet_addr(getIP(host_name, to_string(port_number)).c_str());
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
    perror("bind");
    return 2;
  }
  
  while(1)
    {
      cout << "stuff" << endl;

      // set socket to listen status
      if (listen(sockfd, 1) == -1) {
	perror("listen");
	return 3;
      }

      cout << "stuff1" << endl;

      // accept a new connection
      struct sockaddr_in clientAddr;
      socklen_t clientAddrSize = sizeof(clientAddr);
      cout << "stuff2" << endl;
      int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

      cout << clientSockfd  << endl;

      if (clientSockfd == -1) {
	cout << "stuf3f" << endl;

	perror("accept");
	return 4;
      }
      cout << "stuf3f" << endl;
      char ipstr[INET_ADDRSTRLEN] = {'\0'};
      inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
      std::cout << "Accept a connection from: " << ipstr << ":" <<
	ntohs(clientAddr.sin_port) << std::endl;
      
      thread(resolveRequest, clientAddr, clientSockfd); 
      //every time a connection is made, that connection will get its own thread
    }
}

int resolveRequest(sockaddr_in clientAddr, int clientSockfd)
{
  cout << "butts" <<endl;
  bool isEnd = false;
  char buf[20] = {0};
  stringstream ss;
  
  int timeout = 10;
  int timer = 0;
  HttpRequest message_content;
  string content;
  while (!isEnd)
    {
      memset(buf, '\0', sizeof(buf));
      while (recv(clientSockfd, buf, 20, 0) == -1)
	{
	  timer++;
	  if(timer >= timeout)
	    {
	      close(clientSockfd); //close connection
	    }
	}
      
      ss << buf << endl;
      
      bool has_complete_message = false;
      while(!has_complete_message)
	{
	  content = ss.str();
	  if(content.find("\r\n\r\n") == string::npos)
	    //means you didnt find this sequence so keep polling cause we dont have all data
	    {
	      continue;    //keep polling
	    }
	  else   //we did find the complete string
	    {
	      has_complete_message = true;
	    }
	}
      ss.str("");
    }
  
  message_content.decode(content);
  HttpResponse response;
  if(message_content.getMethod() != "GET")
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
  
  fstream myReadFile;
  message_content.getPath();
  string fileName;
  
  if (dir == ".")
    {
      fileName = message_content.getPath();
    }
  else
    {
      fileName = dir + message_content.getPath();
    }
  

  myReadFile.open(fileName,ios::binary);
  if (myReadFile.fail())
    {
      response.setStatus("404 NOT FOUND");
      response.setBodySize(0);
    }
  else
    {
      // file successfully found
      response.setStatus("200 OK");
      string tmp;
      myReadFile >> tmp;
      response.setBody(tmp);
      response.setBodySize(tmp.size());
    }
  
  response.setResponse(response.getStatus(),response.getBody());
  string briansResponse = response.encode();
  
  if (write(clientSockfd, briansResponse.c_str(), briansResponse.size()) == -1)
    {
      perror("write");
      return 4;
    }
  //get the response back from jack
  //send the response to the client
  close(clientSockfd);
  return 0;
}
