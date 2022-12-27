#include "../RBC/railway.h"

#include <fcntl.h>
#include <string.h>

#include "../common/alloc_macro.h"
#include "../common/string_handlers.h"

/**
 * @brief Connects platforms of railway by list of strings with id's split by
 *        delimiter. The strings must be sorted corresponding to platform_list
 *        in railway.
 *
 * @param railway Pointer to Railway, which already have not connected
 *        platforms.
 * @param id_list Array containing platform_number of strings, each of them
 *        represent array of platform id, delimited by id_delim.
 * @param id_delim Delimiter which delimit id's in id_list.
 * @return Railway* Pointer to Railway, with connected platforms.
 */
static Railway *connect_platforms(Railway *railway, char **id_list,
				  const char *id_delim);

Railway *get_malloc_railway_from(const char *file, const char *platform_delim,
				 const char *platform_detail_delim,
				 const char *platform_id_delim)
{
#include "../railway_manager/default_values.h"
	char *railway_string = get_malloc_string_from(file);
	malloc_macro_def(Railway *, railway, sizeof(*railway));
	railway->platform_number =
		get_number_of_tokens(railway_string, platform_delim);
	char **platform_list = get_malloc_token_list_number(
		railway_string, platform_delim, railway->platform_number);
	free(railway_string);
	malloc_macro(Platform **, railway->platform_list,
		     sizeof(*railway->platform_list) *
			     railway->platform_number);
	malloc_macro_def(char **, connected_id_list,
			 railway->platform_number * sizeof(*connected_id_list));

	for (size_t index = 0; index < railway->platform_number; index++) {
		malloc_macro(Platform *, railway->platform_list[index],
			     sizeof(*railway->platform_list[index]));
		set_platform_from(railway->platform_list[index],
				  platform_list[index], platform_detail_delim,
				  &connected_id_list[index]);
	}
	// Free not as array because values of array are used in railway
	free(platform_list);
	return connect_platforms(railway, connected_id_list, platform_id_delim);
}

static Railway *connect_platforms(Railway *railway, char **id_list,
				  const char *id_delim)
{
	for (size_t index = 0; index < railway->platform_number; index++) {
		size_t ids_number =
			get_number_of_tokens(id_list[index], id_delim);
		railway->platform_list[index]->connected_number = ids_number;
		char **connected_ids = get_malloc_token_list_number(
			id_list[index], id_delim, ids_number);
		railway->platform_list[index]->connected =
			get_platform_list_by_ids(railway->platform_list,
						 railway->platform_number,
						 connected_ids, ids_number);
		free(connected_ids);
	}

	return railway;
}

bool can_transfer_on(const Platform platform)
{
	if (platform.type) {
		return platform.actual_capacity < platform.full_capacity;
	}
	return true;
}

// int main(/* int argc, char const *argv[] */) {
//   Railway *railway =
//       get_malloc_railway_from("railway.txt", "\n", "<platform_data>", ",");
//   // printf("%s,", railway->platform_list[0].connected[0].id);
//   for (size_t i = 0; i < railway->platform_number; i++) {
//     printf("%s\t: ", railway->platform_list[i]->id);
//     for (int j = 0; j < railway->platform_list[i]->connected_number; j++) {
//       printf("%s,", railway->platform_list[i]->connected[j]->id);
//       // printf("%d,", railway->platform_list[i].connected_number);
//       // printf("%p,", railway->platform_list[i].connected[j]);
//     }
//     printf("\b.\n");
//   }
//   return EXIT_SUCCESS;
// }
