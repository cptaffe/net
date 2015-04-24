
// net.c
// contains the implementation of network i/o.

#include "net.h"

// default includes for any program.
#include <cstdlib>
#include <iostream>
#include <stdexcept>
//#include <climits>
#include <cstring>
#include <cerrno>

#include <unistd.h>

net::server::server() {
  std::cout << "initializing server" << std::endl;
}

net::client::client() {
  std::cout << "initializing server" << std::endl;
}

namespace {
  using namespace net;
  struct addrinfo *init_addr(const char *addr, const char *port) {
    struct addrinfo *results, hint = {0};

    // get protocol number for tcp
    struct protoent *proto = getprotobyname("tcp");
    endprotoent(); // close db connection
    if (proto == NULL) {
      throw std::runtime_error("no protocol number for TCP");
    }

    std::cout << proto->p_name << " := " << proto->p_proto << std::endl;

    // configure hint
    hint.ai_family = AF_UNSPEC; // ipv6 & ipv4
    hint.ai_socktype = SOCK_STREAM; // only return streams (not datagrams)
    hint.ai_protocol = proto->p_proto; // the tcp protocol (#6)
    hint.ai_flags = AI_V4MAPPED | AI_ADDRCONFIG; // default flags

    if (addr == NULL) {
      // is a server connection if addr is null.
      hint.ai_flags |= AI_PASSIVE;
    }

    // get information from address and port.
    int err = getaddrinfo(addr, port, &hint, &results);
    if (err != 0) {
      throw std::runtime_error("getaddrinfo errored with: " + std::string(gai_strerror(err)));
    }
    return results;
  }

  void free_addr(struct addrinfo *info) {
    freeaddrinfo(info);
  }

} // namespace

net::io::conn::conn(std::string addr, std::string port) {
  // client connection
  std::cout << "client connection" << std::endl;
}

net::io::conn::conn(std::string port) {
  // server connection
  std::cout << "server connection" << std::endl;

  struct addrinfo *info = NULL;
  try {
    info = init_addr(NULL, port.c_str());
    if (info == NULL) {
      throw std::runtime_error("no address info found");
    }
  } catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl;
    throw e;
  }

  // init socket
  fd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
  int err = bind(fd, info->ai_addr, info->ai_addrlen);
  if (err == -1) {
    throw std::runtime_error("bind errored with: " + std::string(strerror(errno)));
  }

  err = listen(fd, backlog);
  if (err == -1) {
    throw std::runtime_error("listen errored with: " + std::string(strerror(errno)));
  }

  // cleanup
  free_addr(info);

  // testing accept
  for (;;) {
    struct sockaddr naddr = {0};
    socklen_t nsocklen = 0;
    int nfd = accept(fd, &naddr, &nsocklen);
    if (nfd == -1) {
      throw std::runtime_error("accept errored with: " + std::string(strerror(errno)));
    }

    char buf[100] = {0};
    ssize_t rbytes;
    while ((rbytes = read(nfd, &buf, sizeof(buf) - 1)) > 0) {
      // echo server.
      write(nfd, &buf, rbytes);
    }
    close(nfd); // close new file descriptor.
    if (rbytes == -1) {
      throw std::runtime_error("read errored with: " + std::string(strerror(errno)));
    }
  }
}

net::io::conn::~conn() {
  close(fd);
}

net::io::io() : c("8080") {}

// testing entry point
int main() {
  // tests
  try {
    net::server s;
  } catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
}
