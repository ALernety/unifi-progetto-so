#include "../PADRE_TRENI/TRENO.h"
#include "../PADRE_TRENI/PADRE_TRENI.h"
#include "../common/log.h"
#include "../common/socket.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <unistd.h>

#define MICROSECONDS 10000

static int check_next_segment(char *next_segment, char *segment_free);

static void access_segment(int next_segment_fd, char *next_segment,
                           char *cur_segment);

static void free_segment(char *segment);

static int check_next_segment(char *next_segment, char *segment_value) {
  char segment_path[PATH_MAX];
  sprintf(segment_path, "tmp/%s", next_segment);
  int next_segment_fd = open(segment_path, O_RDWR);
  if (next_segment_fd == -1) {
    perror("Error opening a segment file");
    exit(EXIT_FAILURE);
  }
  flock(next_segment_fd, LOCK_SH);
  read(next_segment_fd, segment_value, 1);
  flock(next_segment_fd, LOCK_UN);
  lseek(next_segment_fd, 0, SEEK_SET);
  return next_segment_fd;
}

static void access_segment(int next_segment_fd, char *next_segment,
                           char *cur_segment) {
  flock(next_segment_fd, LOCK_EX);
  file_write(next_segment_fd, "1", 1);
  usleep(MICROSECONDS);
  flock(next_segment_fd, LOCK_UN);
}

static void free_segment(char *segment) {
  char cur_segment_path[PATH_MAX];
  sprintf(cur_segment_path, "tmp/%s", segment);
  int cur_segment_fd = open(cur_segment_path, O_WRONLY);
  flock(cur_segment_fd, LOCK_EX);
  file_write(cur_segment_fd, "0", 1);
  flock(cur_segment_fd, LOCK_UN);
  close(cur_segment_fd);
}

void traverse_itinerary(char **itinerary_list, int log_fd, char *socket_path,
                        const char *train, const char *request_delim) {
  int next_seg_counter = 0;
  char segment_value;
  char *next_segment;
  char *cur_segment = itinerary_list[next_seg_counter++];

  while (1) {
    next_segment = itinerary_list[next_seg_counter];
    log_segment(log_fd, cur_segment, next_segment);
    if (next_segment[0] == 'S') {
      log_segment(log_fd, next_segment, "--");
      // If next_segment is a station, final destination is reached.
      free_segment(cur_segment);
      close(log_fd);
      exit(EXIT_SUCCESS);
    }
    // Open file corresponding to the next segment to enter and check if it is
    // free
    int next_segment_fd = check_next_segment(next_segment, &segment_value);
    if (segment_value == '0') {
      access_segment(next_segment_fd, next_segment, cur_segment);
      if (cur_segment[0] == 'S') {
        // If train is in the departure station, It
        // can immediately enter the next segment
        sprintf(cur_segment, "%s", next_segment);
        close(next_segment_fd);
        next_seg_counter++;
      } else {
        free_segment(cur_segment);
        sprintf(cur_segment, "%s", next_segment);
        next_seg_counter++;
        close(next_segment_fd);
      }
    } else {
      // If segment_value is 1, then the next segment is occupied by
      // another train
      usleep(MICROSECONDS);
    }
  }
}

int create_socket_client(char *socket_path, size_t port) {
  int sfd;
  socket_data socket_input;
  socket_input.socket_path = socket_path;
  socket_input.port = port;
  socket_input.sfd = &sfd;
  socket_input.user = CLIENT;
  return socket_open(socket_input, AF_INET);
}

char *get_itinerary(int sfd, char *train_name) {
  char *itinerary;
  // ask itinerary to REGISTRO
  socket_write(&sfd, train_name, strlen(train_name));
  // get itinerary from REGISTRO
  itinerary = socket_read_malloc(&sfd, "\0");
  return itinerary;
}
