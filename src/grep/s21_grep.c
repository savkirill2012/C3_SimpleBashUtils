#include "s21_grep_lib.h"

int main(int argc, char **argv) {
  // init
  FLAGS_GREP flags;
  FILES_GREP files;
  TEMPLATES_GREP temps;

  int error_flag = 0;

  flags.flag_e = 0;
  flags.flag_i = 0;
  flags.flag_v = 0;
  flags.flag_c = 0;
  flags.flag_l = 0;
  flags.flag_n = 0;
  flags.flag_h = 0;
  flags.flag_s = 0;
  flags.flag_f = 0;
  flags.flag_o = 0;

  files.filenames = NULL;
  files.len = 0;

  temps.templates = NULL;
  temps.len_temp = 0;

  // get_from_input
  error_flag =
      get_flags_files_templates_in_structs(&flags, &files, &temps, argc, argv);

  // cook func
  if (error_flag == 0 || error_flag == 2) {
    cook_grep(files, flags, temps);
  }

  // clean
  if (files.len > 0) {
    for (int i = 0; i < files.len; i++) {
      free(files.filenames[i]);
    }
    free(files.filenames);
  }

  if (temps.len_temp) {
    if (error_flag == 2) {
      temps.len_temp += 1;
    }
    for (int i = 0; i < temps.len_temp; i++) {
      free(temps.templates[i]);
    }
    free(temps.templates);
  }

  return 0;
}