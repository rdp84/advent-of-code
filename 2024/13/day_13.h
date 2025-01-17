#ifndef DAY_13_H
#define DAY_13_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A_TOKENS   3
#define B_TOKENS   1
#define CORRECTION 10000000000000

struct equation {
  int64_t a;
  int64_t b;
  int64_t equals;
};
typedef struct equation equation;

void solve_sim_equation(equation, equation, int64_t *);

#endif
