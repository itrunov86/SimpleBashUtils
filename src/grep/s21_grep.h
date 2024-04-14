#ifndef S21_GREP_H
#define S21_GREP_H

#include <errno.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int cflag;

} T_s21_grep_flags;

typedef struct {
  char *pattern;
  int empty_pattern;
  char *file;
  int index_file;
  regex_t regex;
  regmatch_t matching;
  int count_files;
  int printNameFile;
  int count_line;
  int count_match;
  char *remaining;
  int error;

} T_s21_grep_data;

typedef struct {
  T_s21_grep_data data;
  T_s21_grep_flags flags;
} T_data;

void parcer(int argc, char **argv, T_data *data);
void addArrayString(T_data *data, char *optarg);
void add_patterns_from_file(T_data *data, char *filename);
void fix_patterns(char *patterns);
void compile_regular(T_data *data);
void reader(T_data *data, char **argv);
void setup_logic(T_data *data);
void find_regular(T_data *data, char *line);
void display_matching(char *line, T_data *data);

#endif