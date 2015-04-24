
#include <string>
// cstdint is a c++11 feature, so we use stdint.h
#include <stdint.h>

// includes in anonymous namespace
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace net {
// static class that handles pooling.
class io {
public:
  // static helper functions go here.
  io();

protected:
  // connection class.
  class conn {
  public:
    conn(std::string addr, std::string port);
    conn(std::string port);
    ~conn();
  private:
    // connection internals
    static const int backlog = 1024;
    int fd;
  };
private:
  // data structures
  conn c;
};

class server : io {
public:
  server();
};

class client : io {
public:
  client();
};

} // namespace net
