#include "helper.hpp"

HttpRequest::HttpRequest(string url)
{
  url.erase(0,7);
  for(int i = 0; i < url.length(); i++)
    {
      if(url[i] == "/")
	{

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

int HttpRequest::decode(string message)
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
      else if (message[i] == ":" && count == 3)
	{
	  host = message.substing(pos,i);
	  pos = i;
	  count++;
	}
      else if (message[i] == ":" && count == 4)
	{
	  port = message.substring(pos,i);
	  pos = i;
          count++;
	}
    }
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

string HttpResponse::encode();
{
  message = 

  return message;
}

int HttpResponse::decode(string message)
{
  
}



