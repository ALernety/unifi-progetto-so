#include <sys/types.h>

/**
 * @brief Iterates through all segments that make up the itinerary
 *        and provides a log file with detailed access informations.
 *
 * @param itinerary_list Array of strings obtained by splitting the itinerary
 * @param log_fd Log file descriptor
 * @param socket_path Path to AF_UNIX socket, to connect to RBC. If RBC not
 *        needed will be empty string.
 * @param train Name of train which use itinerary.
 */
void traverse_itinerary(char **itinerary_list, int log_fd, char *socket_path,
                        char *train);

/**
 *@brief Create a client-side AF_INET socket
 *
 *@param socket_path IP address
 *@param port Server socket port
 *@return int Socket file descriptor
 */
int create_socket_client(char *socket_path, size_t port);

/**
 * @brief Get the itinerary needed for the trains via AF-INET socket
 *
 * @param sfd Socket file descriptor
 * @param train_name Name of train
 * @return char* String containing the itinerary
 */
char *get_itinerary(int sfd, char *train_name);
