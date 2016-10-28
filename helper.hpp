#include <string>

class HttpRequest 
{
public:
  void setMethod(string m);
  string getMethod();
  void setVersion(string m);
  string getVersion();
  void setPort(string m);
  string getPort();
  void setPath(string m);
  string getPath();
  void setMessage(string m);
  string getMessage();
  int encode();
  int decode(string message);

private:
  string method;
  string version;
  string port;
  string path;
  string message;
};

class HttpResponse
{
public:
  void setVersion(string m);
  string getVersion();
  void setStatus(string m);
  string getStatus();
  void setBody(string m);
  string getBody();
  void setMessage(string m);
  string getMessage();
  int encode();
  int decode(string message);

private:
  string version;
  string status;
  string body;
  string message;
};
