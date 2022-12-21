#include "../common/alloc_macro.h"
#include "../common/log.h"
#include "../common/parent_dir.h"
#include "../common/string_handlers.h"
#include "PADRE_TRENI.h"
#include "TRENO.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEGMENTS_NUM 16
#define TRAIN_NUM 5

int main(int argc, char *argv[]) {

  int Train[TRAIN_NUM];
  char *itinerary = NULL;
  char train_name[5];
  char ip_address[] = "127.0.0.1";
  char port_string[] = "43210";

  parent_dir_def(project_path, argv[0], 2);
  if (chdir(project_path) == -1) {
    perror("change directory");
    abort();
  }

  umask(000);
  // Get the size of the formatted string to allocate needed memory
  int size = snprintf(NULL, 0, "tmp/MA%d", SEGMENTS_NUM);
  malloc_macro_def(char *, file, (size * sizeof(char)) + 1);
  int log_file_size = snprintf(NULL, 0, "log/T%d", TRAIN_NUM);
  malloc_macro_def(char *, log_file, (log_file_size * sizeof(char)) + 1);

  for (int i = 0; i < SEGMENTS_NUM; i++) {
    segment_create(file, i);
  }
  free(file);
  // create and execute train processes
  for (int i = 0; i < TRAIN_NUM; i++) {
    Train[i] = process_create();
    if (Train[i] == 0) {
      sprintf(log_file, "log/T%i.log", i + 1);
      sprintf(train_name, "T%i", i + 1);
      int sfd = create_socket_client(ip_address, port_string);
      itinerary = get_itinerary(sfd, train_name);
      Train[i] = log_create(log_file);
      // Split the itinerary to get list of segments and stations
      char **itinerary_list = get_malloc_token_list(itinerary, ", ");
      traverse_itinerary(itinerary_list, Train[i]);
    }
  }
  free(log_file);
  for (int i = 0; i < TRAIN_NUM; i++) {
    wait(NULL);
  }
}
