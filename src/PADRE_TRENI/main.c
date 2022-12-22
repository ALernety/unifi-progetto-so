#include "../PADRE_TRENI/PADRE_TRENI.h"
#include "../common/alloc_macro.h"
#include "../common/parent_dir.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEGMENTS_NUM 16
#define TRAIN_NUM 5

int main(int argc, char *argv[]) {
  char help_str[850];
  char *RBC_socket_file = strdup("tmp/rbc");
  char *ip_address = strdup("127.0.0.1");
  size_t port = 43210;
  const char *request_delim = ",";
  const char *itinerary_delim = ", ";

  sprintf(help_str,
          "\033[31mNot enough arguments! Example of use:\033[0m\n"
          "\n"
          "Usage: %s <AF_UNIX_SOCKET> <AF_INET_ADDRESS> <AF_INET_PORT> "
          "<REQUEST_DELIMITER> <ITINERARY_DELIMITER>\n\n"
          "\033[36m<AF_UNIX_SOCKET>\033[0m possible values are:\n"
          "    \033[36mtmp/rbc\033[0m        - Path to RBC socket. To use "
          "without RBC set to empty string.\n"
          "\n"
          "\033[36m<AF_INET_ADDRESS>\033[0m possible values are:\n"
          "    \033[36m127.0.0.1\033[0m      - Is default value\n"
          "\n"
          "\033[36m<AF_INET_PORT>\033[0m possible values are:\n"
          "    \033[36m43210\033[0m          - Is default value\n"
          "\n"
          "\033[36m<REQUEST_DELIMITER>\033[0m possible values are:\n"
          "    \033[36m,\033[0m              - Is default value\n"
          "\n"
          "\033[36m<ITINERARY_DELIMITER>\033[0m possible values are:\n"
          "    \033[36m', '\033[0m           - Is default value\n",
          argv[0]);

  switch (argc) {
  case 6:
    itinerary_delim = argv[5];
    __attribute__((fallthrough));
  case 5:
    request_delim = argv[4];
    __attribute__((fallthrough));
  case 4:
    port = atoi(argv[3]);
    __attribute__((fallthrough));
  case 3:
    ip_address = strdup(argv[2]);
    __attribute__((fallthrough));
  case 2: {
    RBC_socket_file = strdup(argv[1]);
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
    bool is_rbc =
        strcmp(RBC_socket_file, "") && access(RBC_socket_file, F_OK) == -1;
    if (is_rbc) {
      printf("\033[31mRBC socket file not reachable!\033[0m\n");
      usleep(1000000);
      if (access(RBC_socket_file, F_OK) == -1) {
        printf("\033[31mRBC socket file not found!\033[0m\n");
        printf("%s", help_str);
        exit(EXIT_FAILURE);
      }
    }
    break;
  }
  default:
    printf("%s", help_str);
    exit(EXIT_FAILURE);
  }

  parent_dir_def(project_path, argv[0], 2);
  if (chdir(project_path) == -1) {
    perror("change directory");
    abort();
  }

  umask(000);
  // Get the size of the formatted string to allocate needed memory
  int size = snprintf(NULL, 0, "tmp/MA%d", SEGMENTS_NUM);
  char file[size];
  for (int i = 0; i < SEGMENTS_NUM; i++) {
    segment_create(file, i);
  }
  // create and execute train processes
  for (size_t train_index = 0; train_index < TRAIN_NUM; train_index++) {
    create_train_process(train_index, ip_address, port, RBC_socket_file,
                         itinerary_delim, request_delim);
  }
  for (int i = 0; i < TRAIN_NUM; i++) {
    wait(NULL);
  }
}
