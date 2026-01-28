#ifndef DAY_3_H
#define DAY_3_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

struct input {
  int    len;
  char **banks;
};
typedef struct input input;

input alloc_input(void);
void  free_banks(input);
void  print_banks(input);

long  total_joltage(int);

#endif
