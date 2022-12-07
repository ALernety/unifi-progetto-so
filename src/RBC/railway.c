#include "../RBC/railway.h"
#include "../common/alloc_macro.h"
#include "../common/string_handlers.h"
#include <fcntl.h>
#include <string.h>

static Railway *connect_platforms(Railway *railway, char **connected_id_list,
                                  const char *platform_id_delim);

Railway *get_malloc_railway_from(const char *file, const char *platform_delim,
                                 const char *platform_detail_delim,
                                 const char *platform_id_delim) {

  char *railway_string = get_malloc_string_from(file);
  malloc_macro_def(Railway *, railway, sizeof(*railway));
  railway->platform_number =
      get_number_of_tokens(railway_string, platform_delim);
  char **platform_list = get_malloc_token_list_number(
      railway_string, platform_delim, railway->platform_number);
  free(railway_string);
  malloc_macro(Platform *, railway->platform_list,
               sizeof(Platform) * railway->platform_number);
  malloc_macro_def(char **, connected_id_list,
                   railway->platform_number * sizeof(*connected_id_list));

  for (size_t index = 0; index < railway->platform_number; index++) {
    get_platform_from(&railway->platform_list[index], platform_list[index],
                      platform_detail_delim, &connected_id_list[index]);
  }
  // Free not as array because values of array are used in railway
  free(platform_list);
  return connect_platforms(railway, connected_id_list, platform_id_delim);
}

static Railway *connect_platforms(Railway *railway, char **id_list,
                                  const char *id_delim) {
  for (size_t index = 0; index < railway->platform_number; index++) {
    size_t ids_number = get_number_of_tokens(id_list[index], id_delim);
    railway->platform_list[index].connected_number = ids_number;
    char **connected_ids =
        get_malloc_token_list_number(id_list[index], id_delim, ids_number);
    railway->platform_list[index].connected =
        get_connected_platforms_by_id_list(railway->platform_list,
                                           railway->platform_number,
                                           connected_ids, ids_number);
    free(connected_ids);
  }

  return railway;
}

bool can_transfer_on(const Platform platform) {
  if (platform.type) {
    return platform.actual_capacity < platform.full_capacity;
  }
  return true;
}
