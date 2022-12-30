#pragma once
#include <stdbool.h>
#include <sys/types.h>

#include "RBC/platform.h"

/**
 * @brief Is an strcut containing pointer to platform list and platform number
 *        itself.
 *
 * @param platform_list Array of platforms.
 * @param platform_number Total number of platforms in platform_list.
 */
struct railway {
	Platform **platform_list;
	size_t platform_number;
};
typedef struct railway Railway;

/**
 * @brief Get the malloc railway from file containing an structure of railway
 *        delimited by delimiter parameters.
 *
 * @param file Path to the file.
 * @param platform_delim String which delimit platforms in the file.
 * @param platform_detail_delim String which delimit parameters in
 *        platform_string.
 * @param platform_id_delim String which delimit connected platform id's.
 * @return Railway* Railway created by map.
 */
Railway *get_malloc_railway_from(const char *file, const char *platform_delim,
				 const char *platform_detail_delim,
				 const char *platform_id_delim);

/**
 * @brief Returns if platform passed can hold passage of train.
 *
 * @param platform Platform in which should pass train.
 * @return true Train can pass on platform.
 * @return false Platform has not enough capacity to hold passage of train.
 */
bool can_transfer_on(const Platform platform);
