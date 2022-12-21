#include "../PADRE_TRENI/PADRE_TRENI.h"
#include "../common/alloc_macro.h"
#include "../common/parent_dir.h"
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
  char *ip_address = strdup("127.0.0.1");
  size_t port = 43210;

  parent_dir_def(project_path, argv[0], 2);
  if (chdir(project_path) == -1) {
    perror("change directory");
    abort();
  }

  umask(000);
  // Get the size of the formatted string to allocate needed memory
  int size = snprintf(NULL, 0, "tmp/MA%d", SEGMENTS_NUM);
  char file[size];
  for (int i = 0; i < SEGMENTS_NUM; i++) {
    segment_create(file, i);
  }
  // create and execute train processes
  for (size_t train_index = 0; train_index < TRAIN_NUM; train_index++) {
    create_train_process(train_index, ip_address, port);
  }
  for (int i = 0; i < TRAIN_NUM; i++) {
    wait(NULL);
  }
}
