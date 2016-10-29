#include "helper.hpp"

HttpRequest::HttpRequest(string url)
{
  method = "GET";
  version = "HTTP/1.0";

  int count = 0;
  boolean port = false;
  for(int i = 7; i < url.length(); i++)
    {
      if(url[i] == ":")
	{
	  host = url.substring.(7,i);
	  port = true;
	}
      else if(url[i] == ":" && count == 1)
        {
          count++;
        }
      else if(url[i] == "/" && !port)
	{
	  path = url.substring(i, url.length-1);
	  break;
	}
    }
}


void HttpRequest::setMethod(string m)
{
  method = m;
}

string HttpRequest::getMethod();
{
  return method;
}

void HttpRequest::setVersion(string v)
{
  version = v;
}

string HttpRequest::getVersion();
{
  return version;
}

void HttpRequest::setPath(string p)
{
  path = p;
}

string HttpResquest::getMethod();
{
  return p;
}

void HttpRequest::setPort(string p)
{
  port = p;
}

string HttpRequest::getPort();
{
  return port;
}

string HttpRequest::setHost(string h)
{
  host = h;
}

string HttpRequest::getHost()
{
  return host;
}

void HttpRequest::encode()
{
  message = method + " " + path + " " + version + "\r\n" 
    + "Host: " + host + ":" + port + "\r\n\r\n";
  return message;
}

string HttpRequest::decode(string message)
{
  int count = 0;
  int pos = 0;
  for(int i = 0; i < message.length(); i++)
    {
      if(message[i] == " " && count == 0)
	{
	  method = message.substring(0,i);
	  count++;
	  pos = i;
	}
      else if(message[i] == " " && count == 1)
	{
	  path = message.substring();
	  count++;
	  pos = i;
	}
      else if (message[i] == "\r" && count ==2)
	{
	  version = message.substring(pos, i);
	  pos = i;
	  count++;
	}
    }
}

/*////////////////////////////////*/
/*///////////BREAK////////////////*/
/*////////////////////////////////*/


HttpRequest::HttpRequest(string s, string b)
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

string HttpResponse::getVersion();
{
  return version;
}

void HttpResponse::setStatus(string s)
{
  status = s;
}

string HttpResponse::getStatus();
{
  return status;
}

void HttpResponse::setBody(string b)
{
  body = b;
}

string HttpResponse::getBody();
{
  return body;
}

void HttpResponse::setBodySize(int b)
{
  bodySize = b;
}

string HttpResponse::getBodySize();
{
  return bodySize;
}


string HttpResponse::encode();
{
  message = version + " " + status + "\r\n" + "Content-Length: " 
    + to_string(bodySize) + "\r\n\r\n" + body;

  return message;
}

string HttpResponse::decode(string message)
{
  int count = 0;
  int pos = 0;
  for(int i = 0; i < message.length(); i++)
    {
      if(message[i] == " " && count == 0)
        {
          method = message.substring(0,i);
          count++;
          pos = i;
        }
      else if(message[i] == " " && count == 1)
        {
          path = message.substring();
          count++;
          pos = i;
        }
      else if (message[i] == "\r" && count ==2)
        {
          version = message.substring(pos, i);
          pos = i;
          count++;
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
    return 2;
  }
  string ip = ""
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
