#include "../common/alloc_macro.h"
#include "../common/parent_dir.h"
#include "../common/socket.h"
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
  char trainName[5];
  char *project_path = parent_dir(NULL, argv[0], 2);
  char *mode_name;
  char ip_address[] = "127.0.0.1";
  char port_string[] = "43210";
  
  umask(000);
  // Get the size of the formatted string to allocate needed memory
  int size = snprintf(NULL, 0, "%s/tmp/MA%d", project_path, SEGMENTS_NUM);
  malloc_macro_def(char *, file, (size * sizeof(char)) + 1);
  int logFileSize = snprintf(NULL, 0, "%s/log/T%d", project_path, TRAIN_NUM);
  malloc_macro_def(char *, logFile, (logFileSize * sizeof(char)) + 1);
  int itineraryNameSize =
      snprintf(NULL, 0, "%s/tmp/itinerarioT%d", project_path, TRAIN_NUM);
  malloc_macro_def(char *, itineraryName,(itineraryNameSize * sizeof(char)) + 1);
  
 for (int i = 0; i < SEGMENTS_NUM; i++) {
    segment_create(file, project_path,i);
  }
 free(file);
 // create and execute train processes
 for (int i = 0; i < TRAIN_NUM; i++) {
    Train[i] = process_create();
    if (Train[i] == 0) { 
      sprintf(itineraryName, "%s/tmp/itinerarioT%i", project_path, i + 1);
      sprintf(logFile, "%s/log/T%i.log", project_path, i + 1);
      sprintf(trainName, "T%i", i + 1);
      int sfd=create_socket_client(ip_address, port_string);
      itinerary=get_itinerary(sfd,trainName,itineraryName);
      Train[i]=log_create(logFile,i);
      // Split the itinerary to get list of segments and stations
      char **itinerary_list = get_malloc_token_list(itinerary, ", ");
      traverse_itinerary(itinerary_list,Train[i]);
  
    }
  }
  free(itineraryName);
  free(logFile);
  for (int i = 0; i < TRAIN_NUM; i++) {
    wait(NULL);
  }
}
      