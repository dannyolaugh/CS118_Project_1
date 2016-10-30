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
#include <fstream>

#include "helper.h"

using namespace std;

int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "At least one URL is expected";
    return 0;
  }

  for (int i = 1; i < argc; i++) {

    string url = argv[i];
    HttpRequest request;
    request.parseUrl(url);
    // get hostname, port, and IP from url
    string hostname = request.getHost();
    string portNum = request.getPort();
    string ip = getIP(hostname, portNum);
    string file_name = request.getFileName();

    // create a socket using TCP IP
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(portNum.c_str()));     // short, network byte order
    serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    // connect to the server
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
      perror("connect");
      return 2;
    }

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
      perror("getsockname");
      return 3;
    }

    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
    std::cout << "Set up a connection from: " << ipstr << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // struct sockaddr_in addr;
    // addr.sin_family = AF_INET;
    // addr.sin_port = htons(40001);     // short, network byte order
    // addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));
    // if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    //   perror("bind");
    //   return 1;
    // }

    // send/receive data to/from connection
    string request_string = request.encode();
    if (write(sockfd, request_string.c_str(), request_string.size()) == -1) {
      perror("write");
      return 4;
	}

    std::string input;
    char buf[1024] = {0};
    string recv_string;
    int recv_count;

    while (true) {

      memset(buf, '\0', sizeof(buf));

      if ((recv_count = recv(sockfd, buf, 1024, 0)) == -1) {
        perror("recv");
        return 5;
      }
      else if (recv_count == 0) {
        break;
      }
      else {
        recv_string.append(buf, recv_count);
      }
    }

    HttpResponse response;
    response.decode(recv_string);

    if (response.getStatus() == "404 NOT FOUND") {
      cerr << "File requested not found.";
      close(sockfd);
      continue;
    }
    else if (response.getStatus() == "400 BAD REQUEST") {
      cerr << "Server unabled to understand request.";
      close(sockfd);
      continue;
    }
    else if (response.getStatus() == "200 OK") {
      ofstream outf;
      outf.open(file_name);
      if (outf.is_open()) {
        outf << response.getBody();
      }
      outf.close();
    }
    close(sockfd);
  }

  return 0;
}
