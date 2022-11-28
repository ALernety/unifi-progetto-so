#include <stdbool.h>

int file_open(char *filename);
bool file_close(int *fd);
bool file_lock(int fd);
bool file_release(int fd);
int file_read(int fd, char *msg);
int file_write(int fd, char *msg);
