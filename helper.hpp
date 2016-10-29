#include <string>

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
  string encode();
  string decode(string message);

private:
  string method;
  string version;
  string port;
  string path;
  string message;
  string host;
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
  string decode(string message);

private:
  string version;
  string status;
  string body;
  string message;
  int bodySize;
};
