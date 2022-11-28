#include "socket.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define DEFAULT_PROTOCOL 0
#define SOCKET_TYPE SOCK_STREAM

static int socket_open_unix(int *sfd, socket_user user, char *socket_path,
                            int max_connected_clients);
static int socket_open_inet(int *sfd, socket_user user, char *socket_path,
                            unsigned int port, int max_connected_clients);

int socket_open(socket_data socket_input, sa_family_t socket_domain) {
  unlink(socket_input.socket_path);
  switch (socket_domain) {
  case AF_UNIX:
    *socket_input.sfd = socket_open_unix(socket_input.sfd, socket_input.user,
                                         socket_input.socket_path,
                                         socket_input.max_connected_clients);
    break;
  case AF_INET:
    *socket_input.sfd = socket_open_inet(
        socket_input.sfd, socket_input.user, socket_input.socket_path,
        socket_input.port, socket_input.max_connected_clients);
    break;
  }
  return *socket_input.sfd;
}

int socket_accept(int *sfd, int *client_sfd) {
  struct sockaddr_un remote_socket;
  struct sockaddr *remote_socket_ptr = (struct sockaddr *)&remote_socket;
  unsigned int remote_socket_len = sizeof(remote_socket);

  *client_sfd = accept(*sfd, remote_socket_ptr, &remote_socket_len);
  if (*client_sfd == -1) {
    perror("accept");
    abort();
  }
  return *client_sfd;
}

bool socket_close(int *sfd, char *socket_path) {
  int socket_status = close(*sfd);
  if (socket_path != NULL && unlink(socket_path) != -1) {
    perror("unlink");
    abort();
  }
  return socket_status;
}

int socket_read(int *sfd, char *msg, ssize_t msg_len) {
  ssize_t readed_len = read(*sfd, msg, msg_len);
  if (readed_len == -1) {
    perror("read");
    abort();
  }
  return readed_len;
}

int socket_write(int *sfd, char *msg, ssize_t msg_len) {
  int write_len = write(*sfd, msg, msg_len);
  if (write_len < msg_len) {
    perror("write");
    abort();
  }
  return write_len;
}

static int socket_open_unix(int *sfd, socket_user user, char *socket_path,
                            int max_connected_clients) {
  // Define socket, and prepare variables.
  int socketLen;
  struct sockaddr_un socketAddress;
  struct sockaddr *socketAddressPtr;
  socketAddressPtr = (struct sockaddr *)&socketAddress;
  socketLen = sizeof(socketAddress);

  *sfd = socket(AF_UNIX, SOCKET_TYPE, DEFAULT_PROTOCOL);
  socketAddress.sun_family = AF_UNIX;
  snprintf(socketAddress.sun_path, sizeof(socketAddress.sun_path), "%s",
           socket_path);

  switch (user) {
  case CLIENT:
    // Connect to server socket.
    if (connect(*sfd, socketAddressPtr, socketLen) == -1) {
      perror("connect");
      abort();
    }
    break;
  case SERVER:
    // Remove already existing socket.
    if (unlink(socketAddress.sun_path) == -1 && errno != ENOENT) {
      perror("unlink");
      abort();
    }
    // And create new one.
    if (bind(*sfd, socketAddressPtr, socketLen) == -1) {
      perror("bind");
      abort();
    }
    if (listen(*sfd, max_connected_clients) == -1) {
      perror("listen");
      abort();
    }
    break;
  }
  return *sfd;
}

static int socket_open_inet(int *sfd, socket_user user, char *socket_path,
                            unsigned int port, int max_connected_clients) {
  // Define socket and prepare variables.
  int socketLen;
  struct sockaddr_in socketAddress;
  struct sockaddr *socketAddressPtr;
  socketAddressPtr = (struct sockaddr *)&socketAddress;
  socketLen = sizeof(socketAddress);

  *sfd = socket(AF_INET, SOCKET_TYPE, DEFAULT_PROTOCOL);
  socketAddress.sin_family = AF_INET;
  socketAddress.sin_addr.s_addr = inet_addr(socket_path);
  socketAddress.sin_port = htons(port);

  switch (user) {
  case CLIENT:
    // Connect to server socket.
    if (connect(*sfd, socketAddressPtr, socketLen) == -1) {
      perror("connect");
      abort();
    }
    break;
  case SERVER:
    // Create new socket.
    if (bind(*sfd, socketAddressPtr, socketLen) == -1) {
      perror("bind");
      abort();
    }
    if (listen(*sfd, max_connected_clients) == -1) {
      perror("listen");
      abort();
    }
    break;
  }
  return *sfd;
}
