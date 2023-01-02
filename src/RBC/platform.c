#include "RBC/platform.h"

#include <string.h>

#include "common/alloc_macro.h"
#include "common/string_handlers.h"

Platform *set_platform_from(Platform *platform, char *platform_string,
			    const char *delim, char **connected_ids)
{
	char **platform_values_list =
		get_malloc_token_list(platform_string, delim);

	if (sizeof(platform_values_list) < 5) {
		perror("railway format");
		exit(EXIT_FAILURE);
	}

	platform->id = platform_values_list[0];
	platform->type =
		(Platform_Types)get_integer_from(platform_values_list[1]);
	platform->actual_capacity = get_integer_from(platform_values_list[2]);
	platform->full_capacity = get_integer_from(platform_values_list[3]);
	*connected_ids = platform_values_list[4];
	return platform;
}

Platform *get_platform_by_id(Platform **platform_list, size_t platform_number,
			     char *id)
{
	for (size_t index = 0; index < platform_number; index++) {
		if (!strcmp(id, platform_list[index]->id)) {
			return platform_list[index];
		}
	}

	perror("platform with id not found");
	abort();
}

Platform **get_platform_list_by_ids(Platform **platform_list,
				    size_t platform_number,
				    char **connected_ids, size_t ids_number)
{
	malloc_macro_def(Platform **, connected, sizeof(*connected));
	for (size_t index = 0; index < ids_number; index++) {
		realloc_macro(Platform **, connected,
			      (index + 1) * sizeof(*connected));
		connected[index] = get_platform_by_id(
			platform_list, platform_number, connected_ids[index]);
	}

	return connected;
}
