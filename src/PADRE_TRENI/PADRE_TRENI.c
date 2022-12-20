#include "PADRE_TRENI.h"
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int file_create(char *filename);

int process_create() {
  int pid = fork();
  if (pid < 0) {
    perror("Error creating a child process.");
    exit(EXIT_FAILURE);
  }
  return pid;
}

static int file_create(char *filename) {
  int fd;
  if ((fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0666)) < 0) {
    char err_string[30];
    sprintf(err_string, "Error opening file %s", filename);
    perror(err_string);
    exit(EXIT_FAILURE);
  }
  return fd;
}

void segment_create(char *file, int seg_number) {
  sprintf(file, "tmp/MA%d", seg_number + 1);
  int segment = file_create(file);
  file_write(segment, "0", 1);
  close(segment);
}

int file_write(int fd, char *msg, ssize_t msg_len) {
  int bytesWritten = write(fd, msg, msg_len);
  if (bytesWritten != msg_len) {
    perror("Error writing into a file.");
    exit(EXIT_FAILURE);
  }
  return bytesWritten;
}

long file_length(int fd) { return lseek(fd, 0, SEEK_END); }
