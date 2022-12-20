#include "log.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void log_current_date(int fd) {
  char log_message[30];
  size_t date_size;
  struct tm current_time;
  time_t now;

  now = time(NULL);
  current_time = *(localtime(&now));
  date_size = strftime(log_message, 30, "%d/%m/%Y; %H:%M:%S\n", &current_time);
  int write_length = write(fd, log_message, date_size);
  if (write_length != (int)date_size) {
    perror("log date.");
    abort();
  }
}

void log_segment(int fd, char *segment, bool is_current) {
  const char *format_string = is_current ? "[Attuale: %s] " : "[Next: %s] ";
  char log_message[strlen(format_string) + strlen(segment) + 1];

  sprintf(log_message, format_string, segment);
  int write_length = write(fd, log_message, strlen(log_message));
  if (write_length != (int)strlen(log_message)) {
    perror("log segment");
    abort();
  }
}

int log_create(char *log_file) {
  int fd = open(log_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd == -1) {
    perror("log create");
    abort();
  }
  return fd;
}
