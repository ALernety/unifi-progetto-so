#ifndef SRC_RBC_PLATFORM_H_
#define SRC_RBC_PLATFORM_H_
#include <stdbool.h>
#include <sys/types.h>

enum platform_types { STATION, TRACK };
typedef enum platform_types Platform_Types;

/**
 * @brief Platform structure to have easy access to all needed data to passage
 *        of train.
 *
 * @param id Identity platform string.
 * @param type Platform type defined in Platform_Types.
 * @param actual_capacity How much trains already on platform.
 * @param full_capacity Maximum number of train, which platform can hold.
 * @param connected List of connected platforms.
 * @param connected_number Number of connected platforms.
 */
struct platform {
	char *id;
	Platform_Types type;
	size_t actual_capacity;
	size_t full_capacity;
	struct platform **connected;
	int connected_number;
};
typedef struct platform Platform;

/**
 * @brief Set the platform without connecting from platform_string, delimited
 *        by delim and save connected platforms id string to connected_ids.
 *
 * @param platform Platform to set.
 * @param platform_string String containing platform  parameters delimited by
 *        delim.
 * @param delim String which delimit parameters of platform.
 * @param connected_ids Pointer to string where to save the platforms id
 *        connected.
 * @return Platform* Not connected platform.
 */
Platform *set_platform_from(Platform *platform, char *platform_string,
			    const char *delim, char **connected_ids);

/**
 * @brief Get the platform by id string from platform_list, limited by
 *        platform_number.
 *
 * @param platform_list Array of Platforms.
 * @param platform_number Maximum number of Platforms to elaborate or dimension
 *        of platform_list.
 * @param id Identity string of platform.
 * @return Platform* Platform found with id.
 */
Platform *get_platform_by_id(Platform **platform_list, size_t platform_number,
			     char *id);

/**
 * @brief Get platform list by ids from connected_ids.
 *
 * @param platform_list All platforms list.
 * @param platform_number Maximum number of Platforms to elaborate or dimension
 *        of platform_list.
 * @param connected_ids Array of ids of platforms.
 * @param ids_number Number of ids in connected_ids.
 * @return Platform** Array of platforms.
 */
Platform **get_platform_list_by_ids(Platform **platform_list,
				    size_t platform_number,
				    char **connected_ids, size_t ids_number);

#endif // SRC_RBC_PLATFORM_H_
