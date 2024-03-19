#ifndef S21_CAT_INPUT_READ
#define S21_CAT_INPUT_READ

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int flag_b;
  int flag_e;
  int flag_n;
  int flag_s;
  int flag_t;
  int flag_v;
} FLAGS_CAT;

typedef struct {
  char **file_names;
  int len;
} FILES_CAT;

// read input
int get_flags_and_files_from_consol_input(FLAGS_CAT *flags, FILES_CAT *files,
                                          const int num_elem,
                                          char **consol_input);
int add_flags_to_file_struct(FLAGS_CAT *flags, FILES_CAT *files,
                             const char *row);
int add_gnu_flags_to_struct(FLAGS_CAT *flags, const char *row);
void add_file_to_struct(FILES_CAT *files, const char *row);

// realization cat with flags
void cat_the_files_depend_on_flags(FILES_CAT files, FLAGS_CAT flags);
void print_row_with_flags(char *row, int *row_counter, int char_in_a_row,
                          FLAGS_CAT flags);
void print_row_num(int row_num);

// clean
void clean(FILES_CAT *files);

#endif