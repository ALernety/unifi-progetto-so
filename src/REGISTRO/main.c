#include "REGISTRO.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  char helpStr[450];
  char *map_name;
  char *mode_name;

  sprintf(helpStr,
          "\033[31mNot enough arguments! Example of use:\033[0m\n"
          "\n"
          "Usage: %s MODE MAP\n\n"
          "\033[36mMODE\033[0m possible values are:\n"
          "    \033[36mETCS1\033[0m    - send map directly to train\n"
          "    \033[36mETCS2\033[0m    - send map to RBC\n"
          "\n"
          "\033[36mMAP\033[0m possible values are:\n"
          "    \033[36mMAPPA1\033[0m   - manage trains with map one \n"
          "    \033[36mMAPPA2\033[0m   - manage trains with map two\n",
          argv[0]);
  switch (argc) {
  case 3: {
    bool isNotETCS = strcmp(argv[1], "ETCS1") && strcmp(argv[1], "ETCS2");
    bool isNotMAP = strcmp(argv[2], "MAPPA1") && strcmp(argv[2], "MAPPA2");
    if (isNotETCS || isNotMAP) {
      printf("%s", helpStr);
      exit(EXIT_FAILURE);
    }
    mode_name = strdup(argv[1]);
    map_name = strdup(argv[2]);
    break;
  }
  default:
    printf("%s", helpStr);
    exit(EXIT_FAILURE);
    break;
  }

  char *map = get_malloc_map(map_name);
  char **itinerary_list = get_malloc_itinerary_list(map);
  int sfd = create_socket_server("127.0.0.1", 43210, 3);
  start_socket_server(&sfd, itinerary_list);

  return EXIT_SUCCESS;
}