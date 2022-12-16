#include "../RBC/itinerary.h"
#include "../common/alloc_macro.h"
#include "../common/string_handlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Itinerary *get_malloc_itinerary_from(char *itinerary_str, const char *delim,
                                     char *train) {
  malloc_macro_def(Itinerary *, itinerary, sizeof(*itinerary));
  itinerary->train = strdup(train);
  itinerary->platform_number = get_number_of_tokens(itinerary_str, delim);
  itinerary->platform_ids = get_malloc_token_list_number(
      itinerary_str, delim, itinerary->platform_number);
  itinerary->current = 0;
  if (itinerary->platform_number < 2) {
    itinerary->next = 0;
  } else {
    itinerary->next = 1;
  }
  return itinerary;
}

bool move_to_next_platform(Railway *railway, Itinerary *itinerary,
                           char *platform) {
  char *current_id = itinerary->platform_ids[itinerary->current];
  char *next_id = itinerary->platform_ids[itinerary->next];
  if (strcmp(next_id, platform)) {
    return false;
  }
  if (is_on_last_platform_of(*itinerary)) {
    return false;
  }
  Platform current = get_platform_by_id(railway->platform_list,
                                        railway->platform_number, current_id);
  // Check if platforms are connected, and get next platform if they are.
  Platform next =
      get_platform_by_id(current.connected, current.connected_number, next_id);
  if (!can_transfer_on(next)) {
    return false;
  }
  current.actual_capacity--;
  next.actual_capacity++;
  itinerary->current = itinerary->next;
  if (itinerary->platform_number > itinerary->next + 1) {
    itinerary->next++;
  }
  return true;
}

bool is_on_last_platform_of(Itinerary itinerary) {
  if (itinerary.next > itinerary.platform_number) {
    perror("next exceeds platform number");
    abort();
  }
  return itinerary.next == itinerary.current;
}
