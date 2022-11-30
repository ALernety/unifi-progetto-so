#pragma once
#include <stdbool.h>

char *get_malloc_map(char *map_name, char *map);
char **get_malloc_itinerary_list(char *map);
int create_socket_server(char *socket_path, unsigned int port,
                         int max_connected_clients);
void start_socket_server(int *sfd, char **itinerary_list);
char *get_malloc_train(int *sfd);
bool send_itinerary(int *sfd, char **itinerary_list, char *train_name);
