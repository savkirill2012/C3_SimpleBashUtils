#include "s21_grep_lib.h"

int get_flags_files_templates_in_structs(FLAGS_GREP *flags, FILES_GREP *files,
                                         TEMPLATES_GREP *templates, int inp_num,
                                         char **inp_names) {
  int flag_e = 0;
  int flag_f = 0;
  int error_flag = 0;
  int flag_add_temp = 0;
  // logic
  for (int i = 1; i < inp_num; i++) {
    if (inp_names[i][0] == '-' && flag_e == 0 && flag_f == 0) {
      error_flag = add_inp_to_flag_structs(flags, templates, inp_names[i]);
      char *point_to_e = strchr(inp_names[i], 'e');
      char *point_to_f = strchr(inp_names[i], 'f');
      if (point_to_e != NULL && point_to_f != NULL) {
        continue;
      } else if (point_to_e != NULL && strlen(point_to_e) == 1) {
        flag_e = 1;
      } else if (point_to_f != NULL && strlen(point_to_f) == 1) {
        flag_f = 1;
      }
    } else if (flag_e || flag_f || templates->len_temp == 0) {
      if (flag_f) {
        error_flag =
            open_file_and_add_to_templates_struct(templates, inp_names[i]);
        flag_f = 0;
      } else if (flag_e) {
        add_tmp_to_struct(templates, inp_names[i]);
        flag_e = 0;
      } else {
        flag_add_temp = 1;
        add_tmp_to_struct(templates, inp_names[i]);
      }
    } else {
      add_filename_to_file_struct(files, inp_names[i]);
    }

    if (error_flag) {
      break;
    }
  }

  if (flags->flag_e >= 1 && flag_add_temp == 1) {
    remove_temp_to_file_struct(templates, files);
    error_flag = 2;
  }

  return error_flag;
}

void remove_temp_to_file_struct(TEMPLATES_GREP *temp, FILES_GREP *files) {
  char *temp_for_move = temp->templates[0];
  for (int i = 0; i < temp->len_temp - 1; i++) {
    temp->templates[i] = temp->templates[i + 1];
  }
  temp->templates[temp->len_temp - 1] = temp_for_move;
  temp->len_temp -= 1;

  add_filename_to_file_struct(files, temp_for_move);
  if (files->len > 1) {
    temp_for_move = files->filenames[files->len - 1];
    for (int i = files->len - 1; i > 0; i--) {
      files->filenames[i] = files->filenames[i - 1];
    }
    files->filenames[0] = temp_for_move;
  }
  // free(temp_for_move);
}

int add_inp_to_flag_structs(FLAGS_GREP *flags, TEMPLATES_GREP *templates,
                            char *inp_row) {
  int len_row = strlen(inp_row);
  int flag_e = 0;
  int flag_f = 0;
  int error_flag = 0;
  char temp[len_row + 1];
  char filename[len_row + 1];
  int counter = 0;

  for (int i = 1; i < len_row; i++) {
    if (flag_e == 1) {
      temp[counter] = inp_row[i];
      counter += 1;
      temp[counter] = '\0';
    } else if (flag_f == 1) {
      filename[counter] = inp_row[i];
      counter += 1;
      filename[counter] = '\0';
    } else {
      error_flag = search_flag_in_flags_struct(flags, inp_row[i]);
      if (error_flag == 1) {
        break;
      } else if (inp_row[i] == 'e' && i != len_row - 1) {
        flag_e = 1;
      } else if (inp_row[i] == 'f' && i != len_row - 1) {
        flag_f = 1;
      }
    }
  }

  if (flag_e && error_flag == 0) {
    add_tmp_to_struct(templates, temp);
  } else if (flag_f && error_flag == 0) {
    error_flag = open_file_and_add_to_templates_struct(templates, filename);
  }

  return error_flag;
}

int open_file_and_add_to_templates_struct(TEMPLATES_GREP *temp,
                                          const char *filename) {
  int error_flag = 0;

  FILE *sf = fopen(filename, "r");
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
        row[char_in_a_row - 1] = '\0';
        add_tmp_to_struct(temp, row);
        char_in_a_row = 0;
      }

      current_char = fgetc(sf);
    }
    if (char_in_a_row != 0) {
      add_tmp_to_struct(temp, row);
    }
    free(row);
    fclose(sf);
  } else {
    printf("grep: %s: No such file or directory\n", filename);
    error_flag = 1;
  }

  return error_flag;
}

void add_filename_to_file_struct(FILES_GREP *file, const char *filename) {
  int len_add_filename = strlen(filename);

  char **temp_1 = realloc(file->filenames, sizeof(char *) * (1 + file->len));
  if (temp_1 != NULL) {
    file->filenames = temp_1;
    char *temp_2 = malloc(sizeof(char) * (len_add_filename + 1));
    if (temp_2 != NULL) {
      file->filenames[file->len] = temp_2;

      for (int i = 0; i < len_add_filename; i++) {
        file->filenames[file->len][i] = filename[i];
      }
      file->filenames[file->len][len_add_filename] = '\0';

      file->len += 1;
    } else {
      printf("Fail in malloc");
    }
  } else {
    printf("Fail in realloc");
  }
}

void add_tmp_to_struct(TEMPLATES_GREP *temp, const char *add_row) {
  int len_add_row = strlen(add_row);

  char **temp_1 =
      realloc(temp->templates, sizeof(char *) * (1 + temp->len_temp));
  if (temp_1 != NULL) {
    temp->templates = temp_1;
    char *temp_2 = malloc(sizeof(char) * (len_add_row + 1));
    if (temp_2 != NULL) {
      temp->templates[temp->len_temp] = temp_2;

      for (int i = 0; i < len_add_row; i++) {
        temp->templates[temp->len_temp][i] = add_row[i];
      }
      temp->templates[temp->len_temp][len_add_row] = '\0';

      temp->len_temp += 1;
    } else {
      printf("Fail in malloc");
    }
  } else {
    printf("Fail in realloc");
  }
}

int search_flag_in_flags_struct(FLAGS_GREP *flags, const char inp_char) {
  int error_flag = 0;

  switch (inp_char) {
    case 'e':
      flags->flag_e += 1;
      break;
    case 'i':
      flags->flag_i += 1;
      break;
    case 'v':
      flags->flag_v += 1;
      break;
    case 'c':
      flags->flag_c += 1;
      break;
    case 'l':
      flags->flag_l += 1;
      break;
    case 'n':
      flags->flag_n += 1;
      break;
    case 'h':
      flags->flag_h += 1;
      break;
    case 's':
      flags->flag_s += 1;
      break;
    case 'f':
      flags->flag_f += 1;
      break;
    case 'o':
      flags->flag_o += 1;
      break;
    default:
      printf("Can't search flag: %c\n", inp_char);
      error_flag = 1;
  }

  return error_flag;
}

// cook func logic

void cook_grep(FILES_GREP files, FLAGS_GREP flags, TEMPLATES_GREP temps) {
  // loop for files
  for (int i = 0; i < files.len; i++) {
    FILE *sf = fopen(files.filenames[i], "r");
    if (sf != NULL) {
      // init
      char current_char = fgetc(sf);
      char *row = malloc(sizeof(char) * 1);
      int char_in_a_row = 0;
      int row_print_counter = 0;
      int row_counter = 0;
      // logic of work
      while (current_char != EOF) {
        row = realloc(row, sizeof(char) * (char_in_a_row + 2));
        row[char_in_a_row] = current_char;
        char_in_a_row += 1;
        row[char_in_a_row] = '\0';

        if (current_char == '\n') {
          print_row_with_templates_and_flags(files.filenames[i], row,
                                             &row_counter, &row_print_counter,
                                             files.len, flags, temps);
          char_in_a_row = 0;
        }

        current_char = fgetc(sf);
      }
      if (char_in_a_row != 0) {
        if (print_row_with_templates_and_flags(files.filenames[i], row,
                                               &row_counter, &row_print_counter,
                                               files.len, flags, temps)) {
          if (flags.flag_o == 0) {
            printf("\n");
          }
        }
      }
      free(row);
      // logic for flags without print rows
      if (flags.flag_c == 1 && flags.flag_l == 0) {
        if (flags.flag_h == 1 || files.len < 2) {
          printf("%d\n", row_print_counter);
        } else {
          printf("%s:%d\n", files.filenames[i], row_print_counter);
        }
      } else if (flags.flag_l == 1) {
        if (row_print_counter > 0) {
          printf("%s\n", files.filenames[i]);
        }
      }
      fclose(sf);
    } else {
      if (flags.flag_s == 0) {
        printf("grep: %s: No such file or directory\n", files.filenames[i]);
      }
    }
  }
}

int print_row_with_templates_and_flags(char *filename, char *row,
                                       int *row_counter, int *row_print_counter,
                                       int num_filenames_in_file,
                                       FLAGS_GREP flags, TEMPLATES_GREP temps) {
  // init
  int ret_val_if_print = 0;
  int v_flag = 0;
  int counter_same_elem_in_row = 0;

  *row_counter += 1;

  POS_MATCH temps_matches;
  temps_matches.pos_to_match_elems = NULL;
  temps_matches.len = 0;
  temps_matches.mass_of_num_temps = NULL;

  for (int i = 0; i < temps.len_temp; i++) {
    regex_t regs;
    if (flags.flag_i == 0) {
      if (regcomp(&regs, temps.templates[i], REG_NEWLINE) != 0) {
        printf("Error in regcomp func\n");
        break;
      }
    } else {
      if (regcomp(&regs, temps.templates[i], REG_ICASE) != 0) {
        printf("Error in regcomp func\n");
        break;
      }
    }
    if (regexec(&regs, row, 0, NULL, 0) == 0) {
      counter_same_elem_in_row += 1;
    }

    if (flags.flag_o == 1) {
      char *pointer_to_row = row;
      regmatch_t pmatch[100];
      regmatch_t priv_match;

      int counter = 0;
      while (regexec(&regs, pointer_to_row, 1, pmatch, 0) == 0) {
        if (pmatch[0].rm_so != -1) {
          if (counter == 0) {
            add_to_struct(&temps_matches, pmatch[0], i);
          } else {
            regoff_t dif = pmatch[0].rm_eo - pmatch[0].rm_so;
            pmatch[0].rm_so += priv_match.rm_eo;
            pmatch[0].rm_eo = pmatch[0].rm_so + dif;
            add_to_struct(&temps_matches, pmatch[0], i);
          }
          pointer_to_row = row + pmatch[0].rm_eo;
          priv_match = pmatch[0];
          counter += 1;
        }
      }
    }

    regfree(&regs);

    if (flags.flag_o == 0) {
      if (counter_same_elem_in_row > 0) {
        if (flags.flag_v == 0) {
          if (flags.flag_c == 0 && flags.flag_l == 0) {
            print_filename_and_num(flags, filename, num_filenames_in_file,
                                   *row_counter);
            printf("%s", row);
            ret_val_if_print = 1;
          }
          *row_print_counter += 1;
        }
        v_flag = 1;
        break;
      }
    }
  }

  if (flags.flag_o >= 1) {
    if (temps_matches.len > 0) {
      if (flags.flag_v == 0) {
        if (flags.flag_c == 0 && flags.flag_l == 0) {
          sort_logic(&temps_matches);
          for (int i = 0; i < temps_matches.len; i++) {
            print_filename_and_num(flags, filename, num_filenames_in_file,
                                   *row_counter);
            print_for_temps_matches(temps_matches, row, i);
            ret_val_if_print = 1;
          }
        }
        *row_print_counter += 1;
      }
      v_flag = 1;
    }
  }

  if (temps_matches.len > 0) {
    free(temps_matches.pos_to_match_elems);
    free(temps_matches.mass_of_num_temps);
  }

  if (flags.flag_v == 1 && v_flag == 0) {
    if (flags.flag_c == 0 && flags.flag_l == 0 && flags.flag_o == 0) {
      print_filename_and_num(flags, filename, num_filenames_in_file,
                             *row_counter);
      printf("%s", row);
      ret_val_if_print = 1;
    }
    *row_print_counter += 1;
  }

  return ret_val_if_print;
}

void print_for_temps_matches(POS_MATCH temps_matches, char *row, int num) {
  for (int i = temps_matches.pos_to_match_elems[num].rm_so;
       i < temps_matches.pos_to_match_elems[num].rm_eo; i++) {
    printf("%c", row[i]);
  }
  printf("\n");
}

void sort_logic(POS_MATCH *temps_matches) {
  regoff_t min_rm_so = 0;
  int pos_of_lowest_elem = 0;
  int two_equal = 0;

  while (pos_of_lowest_elem < temps_matches->len) {
    min_rm_so = temps_matches->pos_to_match_elems[pos_of_lowest_elem].rm_so;

    for (int i = pos_of_lowest_elem + 1; i < temps_matches->len; i++) {
      if (temps_matches->pos_to_match_elems[i].rm_so < min_rm_so) {
        min_rm_so = temps_matches->pos_to_match_elems[i].rm_so;
        swap_two_elems(temps_matches, pos_of_lowest_elem, i);
      } else if (temps_matches->pos_to_match_elems[i].rm_so == min_rm_so &&
                 i != temps_matches->len) {
        two_equal = 1;
        swap_two_elems(temps_matches, pos_of_lowest_elem + 1, i);
        if (temps_matches->mass_of_num_temps[pos_of_lowest_elem] >
            temps_matches->mass_of_num_temps[pos_of_lowest_elem + 1]) {
          swap_two_elems(temps_matches, pos_of_lowest_elem,
                         pos_of_lowest_elem + 1);
        }
      }
    }

    if (two_equal) {
      pos_of_lowest_elem += 2;
      two_equal = 0;
    } else {
      pos_of_lowest_elem += 1;
    }
  }
}

void swap_two_elems(POS_MATCH *temps_matches, int first_pos, int second_pos) {
  regmatch_t temp = temps_matches->pos_to_match_elems[second_pos];
  temps_matches->pos_to_match_elems[second_pos] =
      temps_matches->pos_to_match_elems[first_pos];
  temps_matches->pos_to_match_elems[first_pos] = temp;

  int temp_pos = temps_matches->mass_of_num_temps[second_pos];
  temps_matches->mass_of_num_temps[second_pos] =
      temps_matches->mass_of_num_temps[first_pos];
  temps_matches->mass_of_num_temps[first_pos] = temp_pos;
}

void add_to_struct(POS_MATCH *temps_matches, regmatch_t pmathc, int num_temps) {
  regmatch_t *temp_1 = realloc(temps_matches->pos_to_match_elems,
                               sizeof(regmatch_t) * (temps_matches->len + 1));
  int *temp_2 = realloc(temps_matches->mass_of_num_temps,
                        sizeof(int) * (temps_matches->len + 1));

  if (temp_1 != NULL) {
    temps_matches->pos_to_match_elems = temp_1;
    temps_matches->pos_to_match_elems[temps_matches->len] = pmathc;
  } else {
    printf("Fail in realloc");
  }

  if (temp_2 != NULL) {
    temps_matches->mass_of_num_temps = temp_2;
    temps_matches->mass_of_num_temps[temps_matches->len] = num_temps;
  } else {
    printf("Fail in realloc");
  }

  if (temp_1 != NULL && temp_2 != NULL) {
    temps_matches->len += 1;
  }
}

void print_filename_and_num(FLAGS_GREP flags, char *filename,
                            int num_filenames_in_file, int row_counter) {
  if (num_filenames_in_file > 1 && flags.flag_h == 0) {
    printf("%s:", filename);
  }

  if (flags.flag_n >= 1) {
    printf("%d:", row_counter);
  }
}