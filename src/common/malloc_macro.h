#define malloc_macro_def_type(def_type, type, variable, size)                  \
  def_type variable = (type)malloc(size + 1);                                  \
  if (!variable) {                                                             \
    perror("Memory allocation failed!\n");                                     \
    abort();                                                                   \
  }

#define malloc_macro(type, variable, size)                                     \
  malloc_macro_def_type(, type, variable, size)

#define malloc_macro_def(type, variable, size)                                 \
  malloc_macro_def_type(type, type, variable, size)
