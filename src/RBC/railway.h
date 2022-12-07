#include "../RBC/platform.h"
#include <stdbool.h>
#include <sys/types.h>

struct railway {
  Platform *platform_list;
  size_t platform_number;
  // size_t station_number;
  // size_t track_number;
};
typedef struct railway Railway;

Railway *get_malloc_railway_from(const char *file, const char *platform_delim,
                                 const char *platform_detail_delim,
                                 const char *platform_id_delim);

bool can_transfer_on(const Platform platform);
