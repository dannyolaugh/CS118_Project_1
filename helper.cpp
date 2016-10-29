#include "helper.hpp"

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

void HttpRequest::setEncodedMessage()
{
  message = method + " " + path + " " + version + "\r\n" 
    + "Host: " + host + ":" + port + "\r\n\r\n";
}

string HttpRequest::getEncodedMessage();
{
  return message;
}

int HttpRequest::decode(string message)
{

}

/*////////////////////////////////*/
/*///////////BREAK////////////////*/
/*////////////////////////////////*/


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

void HttpResponse::setEncodedMessage()
{
  //to do: formulate message                                                             
}

string HttpResponse::getEncodedMessage();
{
  return message;
}

int HttpResponse::decode(string message)
{
  
}



