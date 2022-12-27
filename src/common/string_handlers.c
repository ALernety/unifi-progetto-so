#include "../common/string_handlers.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "../common/alloc_macro.h"

char *get_malloc_string_from(const char *file)
{
	int fd = open(file, O_RDONLY);
	int file_length = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	malloc_macro_def(char *, string_from_file,
			 file_length * sizeof(*string_from_file));
	read(fd, string_from_file, file_length);
	close(fd);
	return string_from_file;
}

int get_number_of_tokens(const char *str, const char *delim)
{
	size_t count = 1;
	size_t delim_length = strlen(delim);
	if (delim_length < 1) {
		delim_length = 1;
	}
	while ((str = strpbrk(str, delim)) != NULL) {
		count++;
		str += delim_length;
	}
	return count;
}

char **get_malloc_token_list(char *string_to_split, const char *delim)
{
	int tokens_number = get_number_of_tokens(string_to_split, delim);
	return get_malloc_token_list_number(string_to_split, delim,
					    tokens_number);
}

char **get_malloc_token_list_number(char *string_to_split, const char *delim,
				    size_t tokens_number)
{
	malloc_macro_def(char **, token_list,
			 tokens_number * sizeof(*token_list));
	char *token = strtok(string_to_split, delim);
	for (size_t index = 0; token && index < tokens_number; index++) {
		malloc_macro(char *, token_list[index],
			     sizeof(char) * strlen(token));
		sprintf(token_list[index], "%s", token);
		token = strtok(NULL, delim);
	}
	return token_list;
}

int get_integer_from(char *string_with_integer)
{
	int extracted_integer = -1;
	// While there are more characters to process...
	while (*string_with_integer) {
		// Found a number
		if (isdigit(*string_with_integer) ||
		    ((*string_with_integer == '-' ||
		      *string_with_integer == '+') &&
		     isdigit(*(string_with_integer + 1)))) {
			// Read number
			extracted_integer = strtol(string_with_integer,
						   &string_with_integer, 10);
			break;
		} else {
			// Otherwise, move on to the next character.
			string_with_integer++;
		}
	}
	return extracted_integer;
}

char *to_upper(char *string_to_handle)
{
	for (size_t char_index = 0; char_index < strlen(string_to_handle);
	     char_index++) {
		string_to_handle[char_index] =
			toupper(string_to_handle[char_index]);
	}
	return string_to_handle;
}
