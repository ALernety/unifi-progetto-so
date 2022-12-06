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
int file_write(int fd, char *msg, ssize_t msg_len);

/**
 * @brief Compute the number of characters in a file.
 *
 * @param fd File descriptor
 * @return long Length of the file
 */
long file_length(int fd);

/**
 * @brief Create a file representing a segment and initialize it with 0.  
 * 
 * @param file Pathname of segment 
 * @param project_path Absolute pathname of project folder 
 * @param seg_number Segment number 
 */
void segment_create(char *file, char *project_path, int seg_number);
