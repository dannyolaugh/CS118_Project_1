#ifndef HELPER_H
#define HELPER_H

#include <string>

class HttpRequest 
{
public:
  void parseUrl(string url);
   
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
  void setResponse(string s, string b);
  
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



HttpRequest::HttpRequest(string url)
{
  method = "GET";
  version = "HTTP/1.0";
  port = "80";

  int pos = 0;
  bool portDef = false;
  bool fileNameFind = false;
  for(int i = 7; i < url.length(); i++)
    {
      if(url[i] == ':')
	{
	  host = url.substr(7,i);
	  pos = i;
	  port = true;
	}
      else if(url[i] == '/' && portDef && !fileNameFind)
        {
	  port = url.substr(pos, i);
	  path = url.substr(i, url.length()-1);
	  fileNameFind = true;
        }
      else if(url[i] == '/' && !portDef && !fileNameFind)
	{
	  host = url.substr(7,i);
	  path = url.substr(i, url.length()-1);
	  fileNameFind = true;
	}
      if(url[i] == '/')
	{
	  pos = i;
	}
    }
  fileName = url.substr(pos, url.length()-1);
}


void HttpRequest::setMethod(string m)
{
  method = m;
}

string HttpRequest::getMethod()
{
  return method;
}

void HttpRequest::setVersion(string v)
{
  version = v;
}

string HttpRequest::getVersion()
{
  return version;
}

void HttpRequest::setPath(string p)
{
  path = p;
}

string HttpRequest::getPath()
{
  return path;
}

void HttpRequest::setPort(string p)
{
  port = p;
}

string HttpRequest::getPort()
{
  return port;
}

void HttpRequest::setHost(string h)
{
  host = h;
}

string HttpRequest::getHost()
{
  return host;
}

void HttpRequest::setFileName(string f)
{
  fileName = f;
}

string HttpRequest::getFileName()
{
  return fileName;
}

string HttpRequest::encode()
{
  message = method + " " + path + " " + version + "\r\n" 
    + "Host: " + host + ":" + port + "\r\n\r\n";
  return message;
}

void HttpRequest::decode(string message)
{
  int count = 0;
  int pos = 0;
  for(int i = 0; i < message.length(); i++)
    {
      if(message[i] == ' ' && count == 0)
	{
	  method = message.substr(0,i);
	  count++;
	  pos = i;
	}
      else if(message[i] == ' ' && count == 1)
	{
	  path = message.substr();
	  count++;
	  pos = i;
	}
      else if (message[i] == '\r' && count ==2)
	{
	  version = message.substr(pos, i);
	  pos = i;
	  count++;
	}
    }
}

/*////////////////////////////////*/
/*///////////BREAK////////////////*/
/*////////////////////////////////*/

HttpResponse::HttpResponse()
{
  string version = "";
  string status = "";
  string body= "";
  string message = "";
  int bodySize = 0;
}

HttpResponse::HttpResponse(string s, string b)
{
  version = "HTTP/1.0";
  status = s;
  body = b;
  bodySize = body.size();
  message = "";
}


void HttpResponse::setVersion(string v)
{
  version = v;
}

string HttpResponse::getVersion()
{
  return version;
}

void HttpResponse::setStatus(string s)
{
  status = s;
}

string HttpResponse::getStatus()
{
  return status;
}

void HttpResponse::setBody(string b)
{
  body = b;
}

string HttpResponse::getBody()
{
  return body;
}

void HttpResponse::setBodySize(int b)
{
  bodySize = b;
}

int HttpResponse::getBodySize()
{
  return bodySize;
}


string HttpResponse::encode();
{
  message = version + " " + status + "\r\n" + "Content-Length: " 
    + to_string(bodySize) + "\r\n\r\n" + body;

  return message;
}

void HttpResponse::decode(string message)
{
  bool statusFound = false;
  bool conLen = false;
  int pos = 0;
  for(int i = 0; i < message.length(); i++)
    {
      if(message[i] == ' ')
        {
          version = message.substr(0,i);
          pos = i;
        }
      else if (message[i] == '\r' && !statusFound)
        {
          status = message.substr(pos, i);
          pos = i;
	  statusFound = true;
	}
      else if(message[i] == ':')
	{
	  if(message.substr(i-14,i) == "Content-Length")
	    {
	      pos = i+1;
	      conLen = true;
	    }
	}
      else if(message[i] == '\r' && conLen && statusFound)
	{
	  conLen = false;
	  bodySize = message.substr(pos, i).size();
	  body = message.substr(pos,i);
	}
    }
}


string getIP(string hostname, string portNum)
{
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
  }

  string ip = "";
  for(struct addrinfo* p = res; p != 0; p = p->ai_next) {
    // convert address to IPv4 address                                      
    struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;

    // convert the IP to a string and print it:                             
    char ipstr[INET_ADDRSTRLEN] = {'\0'};
    inet_ntop(p->ai_family, &(ipv4->sin_addr), ipstr, sizeof(ipstr));
    ip = ipstr;
  }
  free addrinfo(res); // free the linked list     
  return ip;
}

#endif
