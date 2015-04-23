
// net.c
// contains the implementation of network i/o.

// default includes for any program.
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// unix includes
#include <unistd.h>
#include <fcntl.h>

// sockets
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int netinfo(const char *node, short port) {
  int err;

  // a short will be at longest 5 characters (plus null byte).
  char service[6] = {0};

  // initialize service string with port number
  err = sprintf((char *) service, "%d", port);
  if (err < 0 || err > (sizeof(service - 1))) {
    return 1; // error
  }

  struct addrinfo *info;

  err = getaddrinfo(node, (const char *) service, NULL, &info);
  if (err) {
    // gai_strerror returns const char *, which does not need
    // to be freed.
    printf("getaddrinfo errored with: %s\n", gai_strerror(err));
  }

  // do something with info.
  struct addrinfo *info_current = info;
  while (info_current != NULL) {
    // print ip.
    if (info_current->ai_addr != NULL) {
      struct sockaddr_in *addr = (struct sockaddr_in *) info_current->ai_addr;
      char *str = inet_ntoa(addr->sin_addr);
      printf("ip: %s\n", str);
    }

    // continue to next entry.
    info_current = info_current->ai_next;
  }

  freeaddrinfo(info);
}

// test main
int main() {
  return netinfo("google.com", 80);
}
