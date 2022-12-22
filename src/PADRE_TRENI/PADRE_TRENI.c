#include "../PADRE_TRENI/PADRE_TRENI.h"
#include "../PADRE_TRENI/TRENO.h"
#include "../common/alloc_macro.h"
#include "../common/log.h"
#include "../common/string_handlers.h"
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

int file_write(int fd, const char *msg, ssize_t msg_len) {
  int bytes_written = write(fd, msg, msg_len);
  if (bytes_written != msg_len) {
    perror("Error writing into a file.");
    exit(EXIT_FAILURE);
  }
  return bytes_written;
}

long file_length(int fd) { return lseek(fd, 0, SEEK_END); }

void create_train_process(size_t train_index, char *REGISTRO_ip,
                          size_t REGISTRO_port, char *RBC_socket_file,
                          const char *itinerary_delim,
                          const char *request_delim) {
  int pid = fork();
  if (pid < 0) {
    perror("Error creating a child process.");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // Return to parent process.
    return;
  }
  int train_name_size = snprintf(NULL, 0, "T%zu", train_index);
  char train_name[train_name_size];
  sprintf(train_name, "T%zu", train_index + 1);
  char log_file[strlen("log/.log") + train_name_size];
  sprintf(log_file, "log/%s.log", train_name);

  int sfd = create_socket_client(REGISTRO_ip, REGISTRO_port);
  char *itinerary = get_itinerary(sfd, train_name);
  int log_fd = log_create(log_file);
  // Split the itinerary to get list of segments and stations
  char **itinerary_list = get_malloc_token_list(itinerary, itinerary_delim);
  traverse_itinerary(itinerary_list, log_fd, RBC_socket_file, train_name, request_delim);
}
