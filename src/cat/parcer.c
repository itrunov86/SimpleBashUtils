#include "s21_cat.h"

int parcer(int argc, char **argv, opt *options) {
  int opt1;
  int option_index;
  static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                         {"number", 0, 0, 'n'},
                                         {"squeeze-blank", 0, 0, 's'},
                                         {0, 0, 0, 0}};

  while ((opt1 = getopt_long(argc, argv, "benstvTE", long_options,
                             &option_index)) != -1) {
    switch (opt1) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'E':
        options->e = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'T':
        options->t = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      default:
        fprintf(stderr, "invalid option\n");
        return 1;
    }
  }
  return 0;
}