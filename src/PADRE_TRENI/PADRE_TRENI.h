#include <stdbool.h>
#include <sys/types.h>

/**
 * @brief Create a child process using a fork system call.
 *
 * @return int Pid of the newly created process.
 */
int process_create();

/**
 * @brief Write the string msg to file descriptor fd .
 *
 * @param fd File descriptor
 * @param msg String to write
 * @param msg_len Length of the string msg
 * @return int Number of bytes written.
 */
int file_write(int fd, const char *msg, ssize_t msg_len);

/**
 * @brief Compute the number of characters in a file.
 *
 * @param fd File descriptor
 * @return long Length of the file
 */
int64_t file_length(int fd);

/**
 * @brief Create a file representing a segment and initialize it with 0.
 *
 * @param file Pathname of segment
 * @param seg_number Segment number
 */
void segment_create(char *file, int seg_number);

/**
 * @brief Create train process.
 *
 * @param train_index Index of train (train name will be "T{train_index + 1}").
 * @param REGISTRO_ip AF_INET compatible ip address.
 * @param REGISTRO_port AF_INET compatible port.
 * @param RBC_socket_file AF_UNIX socket path.
 * @param itinerary_delim Delimiter for itinerary received from REGISTRO.
 * @param request_delim Delimiter for requests to RBC from RBC_socket_file.
 */
void create_train_process(size_t train_index, char *REGISTRO_ip,
			  size_t REGISTRO_port, char *RBC_socket_file,
			  const char *itinerary_delim,
			  const char *request_delim);
