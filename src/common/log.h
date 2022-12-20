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
 * @param segment Segment name
 * @param is_current It is 0 if logging next segment, 1 if logging current
 * segment
 */
void log_segment(int fd, char *segment, bool is_current);

/**
 * @brief Create log file.
 *
 * @param logFile Pathname of file
 * @return int File descriptor of created file
 */
int log_create(char *logFile);