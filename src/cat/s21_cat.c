#include "s21_cat_input_read.h"

int main(int argc, char **argv) {
  // init
  FILES_CAT input_files;
  FLAGS_CAT input_flags;

  input_files.file_names = NULL;
  input_files.len = 0;

  input_flags.flag_b = 0;
  input_flags.flag_e = 0;
  input_flags.flag_n = 0;
  input_flags.flag_s = 0;
  input_flags.flag_t = 0;
  input_flags.flag_v = 0;

  // main logic
  get_flags_and_files_from_consol_input(&input_flags, &input_files, argc, argv);

  // realization cat with flags
  cat_the_files_depend_on_flags(input_files, input_flags);

  // clean
  clean(&input_files);

  return 0;
}
