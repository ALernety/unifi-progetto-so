#include "../common/socket.h"
#include "../common/alloc_macro.h"

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
#define TIMEOUT_SEC 10
#define TIMEOUT_USEC 0

static int socket_open_unix(int *sfd, socket_user user, char *socket_path,
                            int max_connected_clients);
static int socket_open_inet(int *sfd, socket_user user, char *socket_path,
                            unsigned int port, int max_connected_clients);

int socket_open(socket_data socket_input, sa_family_t socket_domain) {
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
  struct timeval timeout;
  timeout.tv_sec = TIMEOUT_SEC;
  timeout.tv_usec = TIMEOUT_USEC;

  if (setsockopt(*socket_input.sfd, SOL_SOCKET, SO_RCVTIMEO, &timeout,
                 sizeof timeout) < 0) {
    perror("socket timeout receive");
    abort();
  }
  if (setsockopt(*socket_input.sfd, SOL_SOCKET, SO_SNDTIMEO, &timeout,
                 sizeof timeout) < 0) {
    perror("socket timeout send");
    abort();
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

int socket_read_length(int *sfd, char *msg, ssize_t msg_len) {
  ssize_t read_len = read(*sfd, msg, msg_len);
  if (read_len == -1) {
    perror("read");
    abort();
  }
  return read_len;
}

char *socket_read_malloc(int *sfd, const char *end) {
  int end_position = -strlen(end);
  if (end_position > -1) {
    end_position = -1;
  }
  char *message = NULL;
  int message_size = 0;
  do {
    realloc_macro(char *, message, (message_size + 1) * sizeof(char));
    socket_read_length(sfd, &message[message_size++], sizeof(char));
    if (end_position++ < 0) {
      continue;
    }
  } while (strcmp(&message[end_position], end));
  return message;
}

int socket_write(int *sfd, const char *msg, ssize_t msg_len) {
  int write_len = write(*sfd, msg, msg_len + 1);
  if (write_len < msg_len) {
    perror("write");
    abort();
  }
  return write_len;
}

static int socket_open_unix(int *sfd, socket_user user, char *socket_path,
                            int max_connected_clients) {
  // Define socket, and prepare variables.
  int socket_len;
  struct sockaddr_un socket_address;
  struct sockaddr *socket_address_ptr;
  socket_address_ptr = (struct sockaddr *)&socket_address;
  socket_len = sizeof(socket_address);

  *sfd = socket(AF_UNIX, SOCKET_TYPE, DEFAULT_PROTOCOL);
  socket_address.sun_family = AF_UNIX;
  snprintf(socket_address.sun_path, sizeof(socket_address.sun_path), "%s",
           socket_path);

  switch (user) {
  case CLIENT:
    // Connect to server socket.
    if (connect(*sfd, socket_address_ptr, socket_len) == -1) {
      perror("connect");
      abort();
    }
    break;
  case SERVER:
    // Remove already existing socket.
    if (unlink(socket_address.sun_path) == -1 && errno != ENOENT) {
      perror("unlink");
      abort();
    }
    // And create new one.
    if (bind(*sfd, socket_address_ptr, socket_len) == -1) {
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
  int opt = 1;
  int socket_len;
  struct sockaddr_in socket_address;
  struct sockaddr *socket_address_ptr;
  socket_address_ptr = (struct sockaddr *)&socket_address;
  socket_len = sizeof(socket_address);

  *sfd = socket(AF_INET, SOCKET_TYPE, DEFAULT_PROTOCOL);
  // Forcefully attaching socket to the address and port
  if (setsockopt(*sfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    abort();
  }
  socket_address.sin_family = AF_INET;
  socket_address.sin_addr.s_addr = inet_addr(socket_path);
  socket_address.sin_port = htons(port);

  switch (user) {
  case CLIENT:
    // Connect to server socket.
    if (connect(*sfd, socket_address_ptr, socket_len) == -1) {
      perror("connect");
      abort();
    }
    break;
  case SERVER:
    // Create new socket.
    if (bind(*sfd, socket_address_ptr, socket_len) == -1) {
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
