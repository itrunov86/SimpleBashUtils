#include "s21_cat.h"

int main(int argc, char **argv) {
  opt option = {0};
  int error = 0;

  error = parcer(argc, argv, &option);
  if (!error) reader(argv, &option, argc);
  return 0;
}
