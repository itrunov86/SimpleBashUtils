#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;

} opt;

int parcer(int argc, char **argv, opt *options);
int print_symbol(opt *options, int cur, int *print_line, int *prev,
                 int *counter, int *strCount);
int reader(char **argv, opt *options, int argc);

#endif