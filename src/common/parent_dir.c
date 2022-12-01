#include "../common/parent_dir.h"
#include <libgen.h>
#include <stdlib.h>
#include <string.h>

// int main(int argc, char *argv[]) {
//   char parent_dir[strlen(argv[0])];
//   printf("path is \"%s\" argv[0] is \"%s\"\n",
//          parent_dir(parent_dir, argv[0], strtol(argv[1], NULL, 10)),
//          argv[0]);
//   return 0;
// }

char *parent_dir(char *parent_dir, const char *path, unsigned int parent_depth) {
  parent_dir = realpath(path, parent_dir);
  for (unsigned int i = 0; i < parent_depth && strcmp(parent_dir, "/"); i++) {
    parent_dir = dirname(parent_dir);
  }
  return parent_dir;
}
