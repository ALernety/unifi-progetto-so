#include <stdbool.h>
#include <sys/types.h>

struct platform {
  char *id;
  bool type;
  size_t actual_capacity;
  size_t full_capacity;
  struct platform *connected;
  int connected_number;
};
typedef struct platform Platform;

Platform get_platform_from(Platform *platform, char *platform_string,
                           const char *delim, char **connected_ids);

Platform get_platform_by_id(Platform *platform_list, size_t platform_number,
                            char *id);

Platform *get_connected_platforms_by_id_list(Platform *platform_list,
                                             size_t platform_number,
                                             char **connected_ids,
                                             size_t ids_number);
