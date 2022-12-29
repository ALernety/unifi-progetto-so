#include "common/parent_dir.h"

#include <libgen.h>
#include <stdlib.h>
#include <string.h>

char *parent_dir(char *parent_dir, const char *path, unsigned int parent_depth)
{
	parent_dir = realpath(path, parent_dir);
	for (unsigned int i = 0; i < parent_depth && strcmp(parent_dir, "/");
	     i++) {
		parent_dir = dirname(parent_dir);
	}
	return parent_dir;
}
