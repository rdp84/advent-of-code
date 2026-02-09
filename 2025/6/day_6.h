#ifndef DAY_6_H
#define DAY_6_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define SPACE ' '
#define ADD   '+'
#define MUL   '*'

struct input {
  int    rows;
  char **probs;
};
typedef struct input input;

input alloc_input(void);
void  free_input(input);

int  find_sep(input, int);
long solve(long (*fct) (input, int, int));
long solve_row(input, int, int);
long solve_col(input, int, int);
long add(int *, int);
long mul(int *, int);

#endif
