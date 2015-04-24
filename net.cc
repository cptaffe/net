
// net.c
// contains the implementation of network i/o.

// default includes for any program.
#include <cstdlib>
#include <iostream>
#include <cstdint>
#include <climits>
#include <string>
#include <future>

// unix includes
#include <unistd.h>
#include <fcntl.h>

// sockets
namespace net {
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netdb.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
} // namespace net

namespace net {

  // static class that handles pooling.
  class io {
  public:
    // static helper functions go here.

  protected:
    // connection class.
    class conn {
    public:
      conn(std::string &addr, short port);
      ~conn();
    private:
      // connection internals
      sockaddr_in server;
      int fd;
    };
  private:
    // data structures
  };

  class server : io {

  };

  class client : io {

  };

} // namespace net

// testing entry point
int main() {
  // tests
  net::server s;
}
