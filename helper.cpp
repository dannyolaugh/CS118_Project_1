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

void HttpRequest::setMessage(string m)
{
  //to do: formulate message
}

string HttpRequest::getMessage();
{
  return message;
}

int HttpRequest::encode()
{

}

int HttpRequest::decode(string message)
{

}

class HttpResponse
{

  void HttpResponse::setVersion(string v)
  {
    version = v;
  }

  string HttpResponse::getVersion();
  {
    return version;
  }

  void setStatus(string s);
  string getStatus();

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

  void HttpResponse::setMessage(string m)
  {
    //to do: formulate message                                                             
  }

  string HttpResponse::getMessage();
  {
    return message;
  }

  int HttpResponse::encode()
  {

  }

  int HttpResponse::decode(string message)
  {

  }



