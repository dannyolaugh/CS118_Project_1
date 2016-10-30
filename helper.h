
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string>
#include <iostream>
using namespace std;

class HttpRequest 
{
public:
  HttpRequest(string url);
  
  void setMethod(string m);
  string getMethod();
  void setVersion(string v);
  string getVersion();
  void setPort(string p);
  string getPort();
  void setPath(string p);
  string getPath();
  void setHost(string h);
  string getHost();
  void setMessage(string m);
  string getMessage();
  void setFileName(string f);
  string getFileName();
  string encode();
  void decode(string message);

private:
  string method;
  string version;
  string port;
  string path;
  string message;
  string host;
  string fileName;
};

class HttpResponse
{
public:
  HttpResponse(string s, string b);

  void setVersion(string v);
  string getVersion();
  void setStatus(string s);
  string getStatus();
  void setBody(string b);
  string getBody();
  void setBodySize(int b);
  int getBodySize();
  void setMessage(string m);
  string getMessage();
  string encode();
  void decode(string message);

private:
  string version;
  string status;
  string body;
  string message;
  int bodySize;
};


string getIP(string hostname, string portNum);