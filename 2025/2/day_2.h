#ifndef DAY_2_H
#define DAY_2_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NSTR_SIZE 24

struct input {
  int    len;
  long **ranges;
};
typedef struct input input;

input alloc_input(void);
void  free_input(input);
void  print_ranges(input);

#endif
