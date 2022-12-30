#include <stdbool.h>
#include <sys/types.h>

#include "common/mode.h"

/**
 * @brief Iterates through all segments that make up the itinerary
 *        and provides a log file with detailed access informations.
 *
 * @param itinerary_list Array of strings obtained by splitting the itinerary.
 * @param itinerary_number Number of segments in itinerary_list.
 * @param log_fd Log file descriptor.
 * @param socket_path Path to AF_UNIX socket, to connect to RBC. If RBC not
 *        needed will be empty string.
 * @param train Name of train which use itinerary.
 * @param request_delim Request delimiter for delim parameters in RBC.
 */
void traverse_itinerary(char **itinerary_list, size_t itinerary_number,
			int log_fd, char *socket_path, const char *train,
			const char *request_delim);

/**
 * @brief Create a client-side AF_INET socket.
 *
 * @param socket_path IP address.
 * @param port Server socket port.
 * @return int Socket file descriptor.
 */
int create_socket_client(char *socket_path, size_t port);

/**
 * @brief Get the itinerary needed for the trains via AF-INET socket.
 *
 * @param sfd Socket file descriptor.
 * @param train_name Name of train.
 * @return char* String containing the itinerary.
 */
char *get_itinerary(int sfd, char *train_name);

/**
 * @brief Communicate to RBC permit question or notification movement train.
 *
 * @param socket_path Path to AF_UNIX socket of RBC. If empty, return of
 *        function always true.
 * @param train Name of train which ask permit.
 * @param mode Communication mode, instance of Mode.
 * @param request_segment Name of segment which train try to access.
 * @param request_delim Request delimiter for delim parameters in RBC.
 * @return true Passage permited.
 * @return false Passage rejected.
 */
bool communicate_to_rbc(char *socket_path, const char *train, Mode mode,
			const char *request_segment, const char *request_delim);
