#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


void log_current_date(int fd) {
  char buffer[30];
  size_t date_size;
  struct tm tim;
  time_t now;

  now = time(NULL);
  tim = *(localtime(&now));
  date_size = strftime(buffer, 30, "%d/%m/%Y; %H:%M:%S\n", &tim);
  int bytesWritten = write(fd, buffer, date_size);
  if (bytesWritten != (int)date_size) {
    perror("Error logging date.");
    exit(EXIT_FAILURE);
  }
}

 void log_segment(int fd, char *segment, int flag) {
  char buffer[50];
  
  if (flag) {
    sprintf(buffer,"%s %s%s ", "[Attuale:",segment,"] ");
  } else {
   sprintf(buffer,"%s %s%s", "[Next:",segment,"] ");
  }
  int writtenChars = write(fd, buffer, strlen(buffer));
  if (writtenChars != (int)strlen(buffer)) {
    perror("Error logging into file");
    exit(EXIT_FAILURE);
  }
}

int log_create(char *logFile) {
  int fd = open(logFile, O_CREAT | O_WRONLY | O_TRUNC, 0666);
  if (fd == -1) {
    perror("Error creating log file");
    exit(EXIT_FAILURE);
  }
  return fd;

}
