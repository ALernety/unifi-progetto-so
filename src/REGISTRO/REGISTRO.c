#include "REGISTRO.h"
#include "../common/malloc_macro.h"
#include "../common/socket.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int get_number_of_tokens(char *str, char *delim, int *number);

char *get_malloc_map(char *map_name) {
  const char *map_to_use;
  if (!strcmp(map_name, "MAPPA1")) {
    map_to_use = "S1, MA1, MA2, MA3, MA8, S6\n"
                 "S2, MA5, MA6, MA7, MA3, MA8, S6\n"
                 "S7, MA13, MA12, MA11, MA10, MA9, S3\n"
                 "S4, MA14, MA15, MA16, MA12, S8";
  } else if (!strcmp(map_name, "MAPPA2")) {
    map_to_use = "S2, MA5, MA6, MA7, MA3, MA8, S6\n"
                 "S3, MA9, MA10, MA11, MA12, S8\n"
                 "S4, MA14, MA15, MA16, MA12, S8\n"
                 "S6, MA8, MA3, MA2, MA1, S1\n"
                 "S5, MA4, MA3, MA2, MA1, S1";
  } else {
    perror("Wrong map!");
    abort();
  }
  malloc_macro_def(char *, map, strlen(map_to_use));
  sprintf(map, "%s", map_to_use);
  return map;
}

char **get_malloc_itinerary_list(char *map) {
  int number_of_tokens = 0;
  get_number_of_tokens(map, "\n", &number_of_tokens);
  char *itinerary_str = strtok(map, "\n");
  malloc_macro_def(char **, itinerary_list, number_of_tokens * sizeof(char *));
  for (int itinerary_number = 0;
       itinerary_str && itinerary_number < number_of_tokens;
       itinerary_number++) {
    malloc_macro(char *, itinerary_list[itinerary_number],
                 sizeof(char) * strlen(itinerary_str));
    sprintf(itinerary_list[itinerary_number], "%s", itinerary_str);
    itinerary_str = strtok(NULL, "\n");
  }
  return itinerary_list;
}

int create_socket_server(char *socket_path, unsigned int port,
                         int max_connected_clients) {
  int sfd;
  socket_data socket_input;
  socket_input.socket_path = socket_path;
  socket_input.max_connected_clients = max_connected_clients;
  socket_input.port = port;
  socket_input.sfd = &sfd;
  socket_input.user = SERVER;
  return socket_open(socket_input, AF_INET);
}

void start_socket_server(int *sfd, char **itinerary_list) {
  // malloc_macro_def(char **, itinerary_list_t, sizeof(itinerary_list));
  // for (int i = 0; itinerary_list[i]; i++) {
  //   malloc_macro(char *, itinerary_list_t[i], sizeof(itinerary_list[i]));
  //   itinerary_list_t[i] = strdup(itinerary_list[i]);
  // }
  while (true) {
    int client_socket = -1;
    int *client_sfd = &client_socket;
    socket_accept(sfd, client_sfd);
    if (fork() == 0) {
      // printf("%s\n", itinerary_list[0]);
      char *train_name = get_malloc_train(client_sfd);
      send_itinerary(client_sfd, itinerary_list, train_name);
      free(train_name);
      free(itinerary_list);
      socket_close(client_sfd, NULL);
      exit(EXIT_SUCCESS);
    } else {
      socket_close(client_sfd, NULL);
    }
  }
}

char *get_malloc_train(int *sfd) {
  char *train_name;
  int train_name_size = 0;
  do {
    malloc_macro(char *, train_name, (train_name_size + 1) * 8 * sizeof(char));
    socket_read(sfd, &train_name[8 * train_name_size++], 8);
  } while (train_name[(8 * train_name_size) - 1]);
  return train_name;
}

bool send_itinerary(int *sfd, char **itinerary_list, char *train_name) {
  char *train_name_tmp = strdup(train_name);
  int train_index;
  while (*train_name_tmp) { // While there are more characters to process...
    if (isdigit(*train_name_tmp)) {
      // Found a number
      train_index =
          strtol(train_name_tmp, &train_name_tmp, 10) - 1; // Read number
    } else {
      // Otherwise, move on to the next character.
      train_name_tmp++;
    }
  }
  size_t message_len = socket_write(sfd, itinerary_list[train_index],
                                    strlen(itinerary_list[train_index]));
  return message_len == strlen(itinerary_list[train_index]);
}

static int get_number_of_tokens(char *str, char *delim, int *count) {
  *count = 1;
  while ((str = strpbrk(str, delim)) != NULL) {
    (*count)++;
    str++;
  }
  return *count;
}
