#include "../common/mode.h"
#include <stdbool.h>

/**
 * @brief Log current date and time in format DD/MM/YYYY hh/mm/ss
 *
 * @param fd Log file descriptor
 */
void log_current_date(int fd);

/**
 * @brief Log info about next or current segment
 *
 * @param fd Log file descriptor
 * @param current Current segment name.
 * @param next Next segment name.
 * segment
 */
void log_segment(int fd, const char *current, const char *next);

/**
 * @brief Create log file.
 *
 * @param log_file Pathname of file
 * @return int File descriptor of created file
 */
int log_create(char *log_file);

/**
 * @brief Log pass permit of RBC.
 *
 * @param log_fd File descriptor of log file.
 * @param train Train name string.
 * @param current_platform Current platform name string.
 * @param request_platform Request platform name string.
 * @param permit Passage permit on platform.
 * @param mode Type of request to rbc.
 */
void log_rbc(int log_fd, const char *train, const char *current_platform,
             const char *request_platform, const bool permit, Mode mode);
