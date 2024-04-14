#include "s21_cat.h"

int reader(char **argv, opt *options, int argc) {
  int cur;
  int strCount = 0;
  // int emptyCount = 1;
  int counter = 0;
  int prev = '\n';
  int print_line = 0;
  if (argc == 1) {
    printf("s21_cat: no such file\n");
    return 1;
  }
  for (int i = 1; i != argc; i++) {
    if (argv[i][0] != '-' || strcmp(argv[i], "--") == 0 ||
        strcmp(argv[i], "-") == 0) {
      FILE *f = fopen(argv[i], "r");
      if (f) {
        cur = fgetc(f);
        while (cur != EOF) {
          print_symbol(options, cur, &print_line, &prev, &counter, &strCount);
          cur = fgetc(f);
        }
        fclose(f);
      } else {
        fprintf(stderr, "s21_cat: %s no such file\n", argv[i]);
      }
    }
  }
  return 0;
}

int print_symbol(opt *options, int cur, int *print_line, int *prev,
                 int *counter, int *strCount) {
  if (options->s) {
    if (*prev == '\n' && cur == '\n' && *print_line) {
      *print_line = 1;
      *prev = cur;
      return 0;
    } else
      *print_line = 0;
  }
  if (options->b) {
    if (cur != '\n') {
      if (*counter == 0) {
        printf("%6d\t", *strCount = *strCount + 1);
        *counter = 1;
      }
    } else
      *counter = 0;
  }
  if (options->n) {
    if (!(options->b)) {
      if (((cur != '\n') && (*prev == '\n')) || (cur == '\n' && *prev == '\n'))

        printf("%6d\t", *strCount = *strCount + 1);
    }
  }
  if (options->e) {
    if (cur == '\n') printf("$");
  }
  if (options->t) {
    if (cur == '\t') {
      printf("^");
      cur = '\t' + 64;
    }
  }
  if (options->v) {
    if (cur > 127 && cur < 160) printf("M-^");
    if ((cur < 32 && cur != '\n' && cur != '\t') || cur == 127) printf("^");
    if ((cur < 32 || (cur > 126 && cur < 160)) && cur != '\n' && cur != '\t')
      cur = cur > 126 ? cur - 128 + 64 : cur + 64;
    if (cur > 159 && cur < 255) {
      printf("M-");
      cur -= 128;
    }
    if (cur == 255) {
      printf("M-^");
      cur = '?';
    }
  }
  putc(cur, stdout);
  if (*prev == '\n' && cur == '\n')
    *print_line = 1;
  else
    print_line = 0;
  *prev = cur;
  return 0;
}