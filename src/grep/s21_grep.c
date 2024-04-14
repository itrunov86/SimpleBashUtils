#define _GNU_SOURCE
#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    T_data data = {0};
    parcer(argc, argv, &data);
    compile_regular(&data);
    setup_logic(&data);

    if (!data.data.error) {
      for (int i = 0; i < data.data.count_files; ++i) {
        reader(&data, argv);
        data.data.index_file++;
      }
    }
    regfree(&data.data.regex);
    free(data.data.pattern);
  }
  return 0;
}

void parcer(int argc, char **argv, T_data *data) {
  data->flags.cflag |= REG_NEWLINE;
  int rez = 0;

  while ((rez = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (rez) {
      case 'e':
        addArrayString(data, optarg);
        break;
      case 'i':
        data->flags.i = 1;
        data->flags.cflag |= REG_ICASE;
        break;
      case 'v':
        data->flags.v = 1;
        break;
      case 'c':
        data->flags.c = 1;
        break;
      case 'l':
        data->flags.l = 1;
        break;
      case 'n':
        data->flags.n = 1;
        break;
      case 'h':
        data->flags.h = 1;
        break;
      case 's':
        data->flags.s = 1;
        break;
      case 'f':
        add_patterns_from_file(data, optarg);
        data->flags.f = 1;
        break;
      case 'o':
        data->flags.o = 1;
        break;
      default:
        fprintf(stderr, "s21_grep [OPTION]... PATTERNS [FILE]...\n");
        data->data.error = 1;
    }
  }
  if (data->data.empty_pattern == 0) {
    addArrayString(data, argv[optind]);
    optind++;
  }
  if (optind < argc) {
    data->data.index_file = optind;
    data->data.count_files = argc - optind;
  } else
    data->data.error = 1;
}

void addArrayString(T_data *data, char *optarg) {
  if (data->data.empty_pattern == 0) {
    data->data.pattern = (char *)calloc((strlen(optarg) + 4), sizeof(char));
    strcpy(data->data.pattern, optarg);
    data->data.empty_pattern = 1;
  } else {
    data->data.pattern = (char *)realloc(
        data->data.pattern, strlen(data->data.pattern) + strlen(optarg) + 4);
    strcat(data->data.pattern, "\\|");
    strcat(data->data.pattern, optarg);
  }
}

void add_patterns_from_file(T_data *data, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file) {
    char *line = NULL;
    size_t len = 0;
    while ((getline(&line, &len, file)) != -1) {
      size_t line_len = strlen(line);
      if (line[line_len - 1] == '\n') {
        line[line_len - 1] = '\0';
      }
      addArrayString(data, line);
    }
    free(line);
    fclose(file);
  } else {
    fprintf(stderr, "no such file %s\n", filename);
    data->data.error = 1;
  }
}

void compile_regular(T_data *data) {
  int compile;
  compile = regcomp(&data->data.regex, data->data.pattern, data->flags.cflag);
  if (compile != 0) {
    fprintf(stderr, "Error compile regex %s\n", data->data.pattern);
    data->data.error = 1;
  }
}

void setup_logic(T_data *data) {
  if (data->flags.l) {
    data->flags.c = 0;
    data->flags.n = 0;
    data->flags.h = 0;
  }
  if (data->flags.c) data->flags.n = 0;
  if (data->flags.o && data->flags.v)
    if (!data->flags.l && !data->flags.c) data->data.error = 1;
}

void reader(T_data *data, char **argv) {
  char *line = 0;
  size_t length = 0;
  FILE *file = fopen(argv[data->data.index_file], "r");
  if (file) {
    data->data.file = argv[data->data.index_file];
    while (getline(&line, &length, file) != -1 && !data->data.printNameFile) {
      data->data.count_line++;
      size_t line_len = strlen(line);
      if (line[line_len - 1] == '\n') {
        line[line_len - 1] = '\0';
      }
      find_regular(data, line);
    }
    if (data->flags.c) {
      if (data->data.count_files > 1 && !data->flags.h)
        printf("%s:%d\n", data->data.file, data->data.count_match);
      else
        printf("%d\n", data->data.count_match);
    }
    fclose(file);
    free(line);
    data->data.printNameFile = 0;
    data->data.count_match = 0;
    data->data.count_line = 0;
  } else {
    if (!data->flags.s)
      fprintf(stderr, "no such file %s\n", argv[data->data.index_file]);
  }
}

void find_regular(T_data *data, char *line) {
  if (!regexec(&data->data.regex, line, data->flags.o, &data->data.matching,
               0)) {
    if (!data->flags.v) {
      data->data.count_match++;
      if (data->flags.o) {
        data->data.remaining = line + data->data.matching.rm_eo;
        display_matching(line, data);
        while (!regexec(&data->data.regex, data->data.remaining, data->flags.o,
                        &data->data.matching, 0)) {
          display_matching(data->data.remaining, data);
          data->data.remaining =
              data->data.remaining + data->data.matching.rm_eo;
        }
      } else
        display_matching(line, data);
    }
  } else {
    if (data->flags.c && data->flags.v) data->data.count_match++;

    if (data->flags.v) {
      display_matching(line, data);
    }
  }
}

void display_matching(char *line, T_data *data) {
  int skip = 0;
  if (data->data.printNameFile) skip = 1;
  if (!skip && data->flags.l) {
    printf("%s\n", data->data.file);
    data->data.printNameFile = 1;
    skip = 1;
  }

  if (!skip && data->flags.c) {
    skip = 1;
  }
  if (!skip && data->data.count_files > 1 && !data->flags.h)
    printf("%s:", data->data.file);

  if (!skip && data->flags.n) printf("%d:", data->data.count_line);
  if (!skip && data->flags.o) {
    printf("%.*s\n",
           (int)(data->data.matching.rm_eo - data->data.matching.rm_so),
           line + data->data.matching.rm_so);
    skip = 1;
  }
  if (!skip) printf("%s\n", line);
}