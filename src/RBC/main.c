#include "../RBC/RBC.h"
#include "../RBC/itinerary.h"
#include "../common/parent_dir.h"
#include "../common/socket.h"
#include "railway.h"
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void signal_handler(int sig);

int main(int argc, char const *argv[]) {
  char helpStr[450];
  char *ip_address;
  int port = 0;
  char *unix_socket_path;

  sprintf(helpStr,
          "\033[31mWrong use! Example:\033[0m\n"
          "\n"
          "Usage: %s AF_INET_ADDRESS AF_INET_PORT AF_UNIX_ADDRESS\n\n"
          "\033[36mAF_INET_ADDRESS\033[0m possible values are:\n"
          "    \033[36m127.0.0.1\033[0m - is a default value\n"
          "\n"
          "\033[36mAF_INET_PORT\033[0m possible values are:\n"
          "    \033[36m43210\033[0m     - is a default value\n"
          "\n"
          "\033[36mAF_UNIX_ADDRESS\033[0m possible values are:\n"
          "    \033[36mrbc\033[0m       - is a default value\n",
          argv[0]);
  if (argc <= 4) {
    if (argc == 3) {
      ip_address = strdup(argv[1]);
      port = atoi(argv[2]);
      unix_socket_path = strdup("tmp/rbc");
    } else if (argc == 2) {
      ip_address = strdup(argv[1]);
      port = 43210;
      unix_socket_path = strdup("tmp/rbc");
    } else if (argc == 1) {
      ip_address = strdup("127.0.0.1");
      port = 43210;
      unix_socket_path = strdup("tmp/rbc");
    } else {
      ip_address = strdup(argv[1]);
      port = atoi(argv[2]);
      unix_socket_path = strdup(argv[3]);
    }
    struct sockaddr_in sa;
    bool is_wrong_ip = inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0;
    if (is_wrong_ip) {
      printf("\033[31mWrong ip!\033[0m\n");
      printf("%s", helpStr);
      abort();
    }
    if (port == 0) {
      printf("\033[31mWrong port!\033[0m\n");
      printf("%s", helpStr);
      abort();
    }
  } else {
    printf("%s", helpStr);
    abort();
  }

  signal(SIGUSR2, signal_handler);

  parent_dir_def(project_path, argv[0], 2);
  if (chdir(project_path) == -1) {
    perror("change directory");
    abort();
  }

  int sfd;
  socket_data socket_input;
  socket_input.socket_path = ip_address;
  socket_input.port = port;
  socket_input.sfd = &sfd;
  socket_input.user = CLIENT;

  size_t itinerary_number = 0;
  Itinerary **itinerary_list =
      get_malloc_itinerary_list(&itinerary_number, socket_input);
  Railway *railway =
      get_malloc_railway_from("tmp/railway.txt", "\n", "<platform_data>", ",");
  socket_input.socket_path = unix_socket_path;
  socket_input.user = SERVER;
  sfd = socket_open(socket_input, AF_UNIX);
  while (true) {
    char *platform;
    char *train_name;
    int client_sfd = get_request(sfd, ",", &train_name, &platform);
    printf("%s and %s\n", train_name, platform);
    Itinerary *itinerary =
        get_itinerary_by_train(*itinerary_list, itinerary_number, train_name);
    for (size_t jndex = 0; jndex < itinerary->platform_number; jndex++) {
      printf(" %s,", itinerary->platform_ids[jndex]);
    }
    printf("\b.\n");
    if (move_to_next_platform(railway, itinerary, platform)) {
      const char *response = "1";
      socket_write(&client_sfd, response, strlen(response));
    } else {
      const char *response = "0";
      socket_write(&client_sfd, response, strlen(response));
    }
    close(client_sfd);
  }
  return 0;
}

static void signal_handler(int sig) {
  if (sig == SIGUSR2) {
    exit(EXIT_SUCCESS);
  }
  signal(SIGUSR2, signal_handler);
  return;
}
