#include "../RBC/RBC.h"
#include "../RBC/itinerary.h"
#include "../common/log.h"
#include "../common/parent_dir.h"
#include "../common/socket.h"
#include "railway.h"
#include <arpa/inet.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void sigusr2_handler(int sig);

int main(int argc, char const *argv[]) {
  char help_str[1000];

  char *ip_address = strdup("127.0.0.1");
  int port = 43210;
  char *unix_socket_path = strdup("tmp/rbc");
  const char *railway_file = "tmp/railway.txt";
  const char *platform_delim = "\n";
  const char *platform_detail_delim = "<platform_data>";
  const char *platform_id_delim = ",";
  const char *request_delim = ",";

  sprintf(help_str,
          "\033[31mWrong use! Example:\033[0m\n"
          "\n"
          "Usage: %s <AF_INET_ADDRESS> <AF_INET_PORT> <AF_UNIX_ADDRESS> "
          "<RAILWAY_FILE> <PLATFORM_DELIMITER> <PLATFORM_DETAIL_DELIMITER> "
          "<PLATFORM_ID_DELIMITER>\n\n"
          "\033[36m<AF_INET_ADDRESS>\033[0m possible values are:\n"
          "    \033[36m127.0.0.1\033[0m       - is a default value\n"
          "\n"
          "\033[36m<AF_INET_PORT>\033[0m possible values are:\n"
          "    \033[36m43210\033[0m           - is a default value\n"
          "\n"
          "\033[36m<AF_UNIX_ADDRESS>\033[0m possible values are:\n"
          "    \033[36mtmp/rbc\033[0m         - is a default value\n"
          "\n"
          "\033[36m<RAILWAY_FILE>\033[0m possible values are:\n"
          "    \033[36mtmp/railway.txt\033[0m - is a default value\n"
          "\n"
          "\033[36m<PLATFORM_DELIMITER>\033[0m possible values are:\n"
          "    \033[36m\\n\033[0m              - is a default value\n"
          "\n"
          "\033[36m<PLATFORM_DETAIL_DELIMITER>\033[0m possible values are:\n"
          "    \033[36m<platform_data>\033[0m - is a default value\n"
          "\n"
          "\033[36m<PLATFORM_ID_DELIMITER>\033[0m possible values are:\n"
          "    \033[36m,\033[0m               - is a default value\n"
          "\n"
          "\033[36m<REQUEST_DELIMITER>\033[0m possible values are:\n"
          "    \033[36m,\033[0m               - is a default value\n",
          argv[0]);

  switch (argc) {
  case 9:
    request_delim = argv[8];
    __attribute__((fallthrough));
  case 8:
    platform_id_delim = argv[7];
    __attribute__((fallthrough));
  case 7:
    platform_detail_delim = argv[6];
    __attribute__((fallthrough));
  case 6:
    platform_delim = argv[5];
    __attribute__((fallthrough));
  case 5:
    railway_file = argv[4];
    __attribute__((fallthrough));
  case 4:
    unix_socket_path = strdup(argv[3]);
    __attribute__((fallthrough));
  case 3:
    port = atoi(argv[2]);
    __attribute__((fallthrough));
  case 2:
    ip_address = strdup(argv[1]);
    __attribute__((fallthrough));
  case 1: {
    struct sockaddr_in sa;
    bool is_wrong_ip = inet_pton(AF_INET, ip_address, &(sa.sin_addr)) == 0;
    if (is_wrong_ip) {
      printf("\033[31mWrong ip!\033[0m\n");
      printf("%s", help_str);
      exit(EXIT_FAILURE);
    }
    if (port == 0) {
      printf("\033[31mWrong port!\033[0m\n");
      printf("%s", help_str);
      exit(EXIT_FAILURE);
    }
    if (access(railway_file, R_OK) == -1) {
      printf("\033[31mRailway file not found or can't read!\033[0m\n");
      printf("%s", help_str);
      exit(EXIT_FAILURE);
    }
    break;
  }
  default:
    printf("%s", help_str);
    exit(EXIT_FAILURE);
  }

  signal(SIGUSR2, sigusr2_handler);

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
  Railway *railway = get_malloc_railway_from(
      railway_file, platform_delim, platform_detail_delim, platform_id_delim);
  socket_input.socket_path = unix_socket_path;
  socket_input.user = SERVER;
  sfd = socket_open(socket_input, AF_UNIX);
  int log_fd = log_create("log/RBC.log");
  while (true) {
    char *request_platform;
    Mode mode;
    char *train_name;
    int client_sfd =
        get_request(sfd, request_delim, &train_name, &mode, &request_platform);
    Itinerary *itinerary =
        get_itinerary_by_train(itinerary_list, itinerary_number, train_name);
    char *current_platform = itinerary->platform_ids[itinerary->current];
    bool request_result = false;
    switch (mode) {
    case PERMIT:
      request_result =
          permit_to_next_platform(railway, itinerary, request_platform);
      break;
    case MOVE:
      request_result = free_current_platform(railway, itinerary);
      break;
    default:
      break;
    }
    log_rbc(log_fd, train_name, current_platform, request_platform,
            request_result, mode);
    if (request_result) {
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

static void sigusr2_handler(int sig) {
  printf("Terminate RBC.\n");
  exit(EXIT_SUCCESS);
  return;
}
