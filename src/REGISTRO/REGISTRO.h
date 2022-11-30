#pragma once
#include <stdbool.h>

/**
 * @brief Get the malloc map string, using map_name.
 *
 * @param map_name Is a string, to choose predefined map.
 * @return char* Is a string, containing the itineraries of trains,
 *        separated by new line (\n), allocated with malloc.
 */
char *get_malloc_map(char *map_name);

/**
 * @brief Get the malloc itinerary list from map string.
 *
 * @param map Is a string, containing the itineraries of trains,
 *        separated by new line (\n).
 * @return char** Is an array of itineraries, index of which is
 *         a number of line in map, starting count from 0, and
 *         allocated with malloc.
 */
char **get_malloc_itinerary_list(char *map);

/**
 * @brief Create a socket server of AF_INET type.
 *
 * @param socket_path Is a string with IP address.
 * @param port Is a unsigned integer, indicating a port to connect.
 * @param max_connected_clients Maximum number of clients, that this
 *        socket can serve at the same time.
 * @return int It is a socket file descriptor created by function.
 */
int create_socket_server(char *socket_path, unsigned int port,
                         int max_connected_clients);

/**
 * @brief Start receive requests from clients, and give requested
 *        itinerary.
 *
 * @param sfd Is a pointer to the socket file descriptor.
 * @param itinerary_list Is an array of itineraries.
 */
void start_socket_server(int *sfd, char **itinerary_list);

/**
 * @brief Get the malloc train get train name from socket.
 *
 * @param sfd Is a socket file descriptor.
 * @return char* Is a string, containing a train name.
 */
char *get_malloc_train(int *sfd);

/**
 * @brief Send requested itinerary to train.
 *
 * @param sfd Is a socket file descriptor, needed to communicate with train.
 * @param itinerary_list Is an array strings containing of itineraries.
 * @param train_name Is a string with train name.
 * @return true All characters of itinerary was write to socket.
 * @return false Some characters was not write to socket.
 */
bool send_itinerary(int *sfd, char **itinerary_list, char *train_name);
