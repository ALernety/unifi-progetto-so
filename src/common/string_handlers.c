#include "../common/string_handlers.h"
#include "../common/alloc_macro.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *get_malloc_string_from(const char *file) {
  int fd = open(file, O_RDONLY);
  int file_length = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  malloc_macro_def(char *, string_from_file,
                   file_length * sizeof(*string_from_file));
  read(fd, string_from_file, file_length);
  close(fd);
  // const char *map_to_use;
  // if (!strcmp(file, "MAPPA1")) {
  //   map_to_use = "S1, MA1, MA2, MA3, MA8, S6\n"
  //                "S2, MA5, MA6, MA7, MA3, MA8, S6\n"
  //                "S7, MA13, MA12, MA11, MA10, MA9, S3\n"
  //                "S4, MA14, MA15, MA16, MA12, S8";
  // } else if (!strcmp(file, "MAPPA2")) {
  //   map_to_use = "S2, MA5, MA6, MA7, MA3, MA8, S6\n"
  //                "S3, MA9, MA10, MA11, MA12, S8\n"
  //                "S4, MA14, MA15, MA16, MA12, S8\n"
  //                "S6, MA8, MA3, MA2, MA1, S1\n"
  //                "S5, MA4, MA3, MA2, MA1, S1";
  // } else {
  //   perror("Wrong map!");
  //   abort();
  // }
  // malloc_macro_def(char *, string_from_file,
  //                  strlen(map_to_use) * sizeof(*string_from_file));
  // sprintf(string_from_file, "%s", map_to_use);
  return string_from_file;
}

int get_number_of_tokens(const char *str, const char *delim) {
  int count = 1;
  while ((str = strpbrk(str, delim)) != NULL) {
    count++;
    str++;
  }
  return count;
}

char **get_malloc_token_list(char *string_to_split, const char *delim) {
  int tokens_number = get_number_of_tokens(string_to_split, delim);
  return get_malloc_token_list_number(string_to_split, delim, tokens_number);
}

char **get_malloc_token_list_number(char *string_to_split, const char *delim,
                                    size_t tokens_number) {
  malloc_macro_def(char **, token_list, tokens_number * sizeof(*token_list));
  char *token = strtok(string_to_split, delim);
  for (size_t index = 0; token && index < tokens_number; index++) {
    malloc_macro(char *, token_list[index], sizeof(char) * strlen(token));
    sprintf(token_list[index], "%s", token);
    token = strtok(NULL, delim);
  }
  return token_list;
}

int get_integer_from(char *string_with_integer) {
  int extracted_integer = -1;
  // While there are more characters to process...
  while (*string_with_integer) {
    // Found a number
    if (isdigit(*string_with_integer) ||
        ((*string_with_integer == '-' || *string_with_integer == '+') &&
         isdigit(*(string_with_integer + 1)))) {
      // Read number
      extracted_integer = strtol(string_with_integer, &string_with_integer, 10);
      break;
    } else {
      // Otherwise, move on to the next character.
      string_with_integer++;
    }
  }
  return extracted_integer;
}

char *to_upper(char *string_to_handle) {
  for (size_t char_index = 0; char_index < strlen(string_to_handle);
       char_index++) {
    string_to_handle[char_index] = toupper(string_to_handle[char_index]);
  }
  return string_to_handle;
}
