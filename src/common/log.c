#include "log.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

static char *get_date(char *date_string, size_t *date_size);

void log_current_date(int fd) {
  size_t date_size = 29;
  char date_string[date_size];
  char date[date_size + 1];
  get_date(date_string, &date_size);
  sprintf(date, "%s\n", date_string);
  int write_length = write(fd, date, strlen(date));
  if (write_length != (int)date_size + 1) {
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
  int fd = open(log_file, O_CREAT | O_WRONLY , 0644);
  if (fd == -1) {
    perror("log create");
    abort();
  }
  return fd;
}

void log_rbc(int log_fd, const char *train, const char *current_platform,
             const char *request_platform, const bool permit) {
  const char *format_string = "[TRENO RICHIEDENTE AUTORIZZAZIONE: %s], "
                              "[SEGMENTO ATTUALE: %s], "
                              "[SEGMENTO RICHIESTO: %s], "
                              "[AUTORIZZATO: %s], "
                              "[DATA: %s]\n";
  size_t log_length = strlen(format_string) + strlen(train) +
                      strlen(current_platform) + strlen(request_platform) +
                      /*authorize*/ 2 + 1;
  char log_message[log_length];

  size_t date_size = 30;
  char date[date_size];
  get_date(date, &date_size);

  sprintf(log_message, format_string, train, current_platform, request_platform,
          permit ? "SI" : "NO", date);
  int write_length = write(log_fd, log_message, strlen(log_message));
  if (write_length != (int)strlen(log_message)) {
    perror("log segment");
    abort();
  }
}

static char *get_date(char *date_string, size_t *date_size) {
  struct tm current_time;
  time_t now;

  static char *date_format = "%d/%m/%Y; %H:%M:%S";
  now = time(NULL);
  current_time = *(localtime(&now));
  *date_size = strftime(date_string, *date_size, date_format, &current_time);
  return date_string;
}
