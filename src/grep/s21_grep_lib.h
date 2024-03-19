#ifndef S21_GREP_LIB_H
#define S21_GREP_LIB_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int flag_e;
  int flag_i;
  int flag_v;
  int flag_c;
  int flag_l;
  int flag_n;
  int flag_h;
  int flag_s;
  int flag_f;
  int flag_o;
} FLAGS_GREP;

typedef struct {
  char **filenames;
  int len;
} FILES_GREP;

typedef struct {
  char **templates;
  int len_temp;
} TEMPLATES_GREP;

typedef struct {
  int *mass_of_num_temps;
  regmatch_t *pos_to_match_elems;
  int len;
} POS_MATCH;

// input
int search_flag_in_flags_struct(FLAGS_GREP *flags, const char inp_char);
int get_flags_files_templates_in_structs(FLAGS_GREP *flags, FILES_GREP *files,
                                         TEMPLATES_GREP *templates, int inp_num,
                                         char **inp_names);
void add_filename_to_file_struct(FILES_GREP *file, const char *filename);
int add_inp_to_flag_structs(FLAGS_GREP *flags, TEMPLATES_GREP *templates,
                            char *inp_row);
int open_file_and_add_to_templates_struct(TEMPLATES_GREP *temp,
                                          const char *filename);
void add_tmp_to_struct(TEMPLATES_GREP *temp, const char *add_row);
void remove_temp_to_file_struct(TEMPLATES_GREP *temp, FILES_GREP *files);

// cook func
void cook_grep(FILES_GREP files, FLAGS_GREP flags, TEMPLATES_GREP temps);
int print_row_with_templates_and_flags(char *filename, char *row,
                                       int *row_counter, int *row_print_counter,
                                       int num_filenames_in_file,
                                       FLAGS_GREP flags, TEMPLATES_GREP temps);
void print_filename_and_num(FLAGS_GREP flags, char *filename,
                            int num_filenames_in_file, int row_counter);
void sort_logic(POS_MATCH *temps_matches);
void swap_two_elems(POS_MATCH *temps_matches, int first_pos, int second_pos);
void add_to_struct(POS_MATCH *temps_matches, regmatch_t pmathc, int num_temps);
void print_for_temps_matches(POS_MATCH temps_matches, char *row, int num);

#endif