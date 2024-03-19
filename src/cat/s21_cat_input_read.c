#include "s21_cat_input_read.h"

// read input

int get_flags_and_files_from_consol_input(FLAGS_CAT *flags, FILES_CAT *files,
                                          const int num_elem,
                                          char **consol_input) {
  int error_flag = 0;

  for (int i = 1; i < num_elem; i++) {
    if (consol_input[i][0] == '-') {
      if (consol_input[i][1] == '-') {
        error_flag = add_gnu_flags_to_struct(flags, consol_input[i]);
      } else {
        error_flag = add_flags_to_file_struct(flags, files, consol_input[i]);
      }
    } else {
      add_file_to_struct(files, consol_input[i]);
    }

    if (error_flag == -1 && files->file_names == NULL) {
      break;
    }
  }

  return error_flag;
}

int add_flags_to_file_struct(FLAGS_CAT *flags, FILES_CAT *files,
                             const char *row) {
  int error_flag = 0;
  int len = strlen(row);

  for (int i = 1; i < len; i++) {
    int loc_error_flag = 0;

    switch (row[i]) {
      case 'e':
        flags->flag_e += 1;
        flags->flag_v += 1;
        break;
      case 'E':
        flags->flag_e += 1;
        break;
      case 'b':
        flags->flag_b += 1;
        break;
      case 'n':
        flags->flag_n += 1;
        break;
      case 's':
        flags->flag_s += 1;
        break;
      case 't':
        flags->flag_t += 1;
        flags->flag_v += 1;
        break;
      case 'T':
        flags->flag_t += 1;
        break;
      case 'v':
        flags->flag_v += 1;
        break;
      default:
        loc_error_flag = -1;
    }

    if (loc_error_flag == -1) {
      printf("Error can't search the flag %c\n", row[i]);
      error_flag = loc_error_flag;
      if (files->file_names == NULL) {
        break;
      }
    }
  }

  return error_flag;
}

int add_gnu_flags_to_struct(FLAGS_CAT *flags, const char *row) {
  int error_flag = -1;
  char *gnu_flags[] = {"--number-nonblank", "--number", "--squeeze-blank"};

  for (int i = 0; i < 3; i++) {
    if (strcmp(row, gnu_flags[i]) == 0) {
      switch (i) {
        case 0:
          flags->flag_b += 1;
          break;
        case 1:
          flags->flag_n += 1;
          break;
        case 2:
          flags->flag_s += 1;
          break;
      }

      error_flag = 0;
      break;
    }
  }

  if (error_flag == -1) {
    printf("Can't find this flag: %s\n", row);
  }

  return error_flag;
}

void add_file_to_struct(FILES_CAT *files, const char *row) {
  size_t len_row = strlen(row);

  files->file_names =
      realloc(files->file_names, sizeof(char *) * (files->len + 1));
  files->file_names[files->len] = malloc(sizeof(char) * (len_row + 1));

  for (size_t i = 0; i < len_row; i++) {
    files->file_names[files->len][i] = row[i];
  }
  files->file_names[files->len][len_row] = '\0';

  files->len += 1;
}

// realization cat with flags

void cat_the_files_depend_on_flags(FILES_CAT files, FLAGS_CAT flags) {
  // loop for files
  int row_counter = 0;

  for (int i = 0; i < files.len; i++) {
    FILE *sf = fopen(files.file_names[i], "r");
    if (sf != NULL) {
      // init
      char current_char = fgetc(sf);
      char *row = malloc(sizeof(char) * 1);
      int char_in_a_row = 0;
      // logic of work
      while (current_char != EOF) {
        row = realloc(row, sizeof(char) * (char_in_a_row + 2));
        row[char_in_a_row] = current_char;
        char_in_a_row += 1;
        row[char_in_a_row] = '\0';

        if (current_char == '\n') {
          print_row_with_flags(row, &row_counter, char_in_a_row, flags);
          char_in_a_row = 0;
        }

        current_char = fgetc(sf);
      }
      print_row_with_flags(row, &row_counter, char_in_a_row, flags);
      free(row);
      fclose(sf);
    } else {
      if (i > 0) {
        printf("\n");
      }
      printf("Error in open file: %s", files.file_names[i]);
    }
  }
}

void print_row_with_flags(char *row, int *row_counter, int char_in_a_row,
                          FLAGS_CAT flags) {
  static int multiply_empty_row = 0;

  // check for an empty row
  int is_not_empty_row = 0;
  if (char_in_a_row > 1 || row[0] != '\n') {
    is_not_empty_row = 1;
  }

  for (int i = 0; i < char_in_a_row; i++) {
    if (multiply_empty_row == 1 && is_not_empty_row == 0 &&
        flags.flag_s >= 1) {  // flag s realization
      break;
    } else {
      if (i == 0 && row[char_in_a_row] != '\n') {
        if (flags.flag_b >= 1 && is_not_empty_row) {  // flag b realization
          *row_counter += 1;
          print_row_num(*row_counter);
        } else if (flags.flag_n >= 1 &&
                   flags.flag_b < 1) {  // flag n realization
          *row_counter += 1;
          print_row_num(*row_counter);
        }
      }

      if (i == char_in_a_row - 1 && flags.flag_e >= 1 &&
          row[i] == '\n') {  // flag e realization
        printf("$");
      }

      if (flags.flag_v >= 1) {  // flag v realization
        if (((int)row[i] >= 0 && (int)row[i] < 9) ||
            ((int)row[i] >= 11 && (int)row[i] < 32)) {
          printf("^%c", (char)((int)row[i] + 64));
        } else if ((int)row[i] == 9 && flags.flag_t >= 1) {
          printf("^%c", (char)((int)row[i] + 64));
        } else if ((int)row[i] == 127) {
          printf("^%c", (char)((int)row[i] - 64));
        } else {
          printf("%c", row[i]);
        }
      } else {
        if (flags.flag_t >= 1 && ((int)row[i] == 9)) {  // flag t realization
          printf("^%c", (char)((int)row[i] + 64));
        } else {
          printf("%c", row[i]);
        }
      }
    }
  }

  // prep for flag e
  if (is_not_empty_row == 0) {
    multiply_empty_row = 1;
  } else {
    multiply_empty_row = 0;
  }
}

void print_row_num(int row_num) {
  int num = row_num;
  int mas_nums[7];
  int counter = 0;

  while (num > 0) {
    mas_nums[counter] = num % 10;
    num = num / 10;
    counter += 1;
  }

  for (int i = 0; i < 6 - counter; i++) {
    printf(" ");
  }

  for (int i = counter; i > 0; i--) {
    printf("%d", mas_nums[i - 1]);
  }

  printf("\t");
}

// clean

void clean(FILES_CAT *files) {
  for (int i = files->len; i > 0; i--) {
    free(files->file_names[i - 1]);
  }

  free(files->file_names);
}
