#pragma once
#include "../RBC/itinerary.h"
#include "../common/socket.h"
#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief Get the malloc itinerary list from connecting to REGISTRO.
 *
 * @param itinerary_number Pointer to integer to save number of itineraries in
 *        list.
 * @param socket_input Needed data to connect to REGISTRO.
 * @return Itinerary** List of Itinerary's.
 */
Itinerary **get_malloc_itinerary_list(size_t *itinerary_number,
                                      socket_data socket_input);

/**
 * @brief Create a socket server to guide trains.
 *
 * @param socket_path Unix socket path.
 * @param max_connected_clients Number of maximum members in socket queue.
 * @return int Socket file descriptor.
 */
int create_socket_server(char *socket_path, int max_connected_clients);

/**
 * @brief Get the request from train.
 *
 * @param sfd Server socket file descriptor.
 * @param delim Delimiter of train_name and platform
 * @param train_name Train identity string.
 * @param platform Platform id string.
 * @return int Client socket file descriptor.
 */
int get_request(int sfd, const char *delim, char **train_name, char **platform);

/**
 * @brief Get the itinerary by train identity string.
 *
 * @param itinerary_list All itineraries list.
 * @param itinerary_number Number of itineraries in itinerary_list.
 * @param train Identity string of itinerary.
 * @return Itinerary* Itinerary found by train.
 */
Itinerary *get_itinerary_by_train(Itinerary **itinerary_list,
                                  size_t itinerary_number, char *train);

