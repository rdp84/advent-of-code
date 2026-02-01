#ifndef DAY_4_H
#define DAY_4_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROLL  '@'
#define EMPTY '.'

#define roll_at_index(grid, i, j) (*(*(grid + i) + j) == ROLL) ? 1 : 0

#define roll_at_n(grid, i, j)  roll_at_index(grid, i-1, j)
#define roll_at_ne(grid, i, j) roll_at_index(grid, i-1, j+1)
#define roll_at_e(grid, i, j)  roll_at_index(grid, i, j+1)
#define roll_at_se(grid, i, j) roll_at_index(grid, i+1, j+1)
#define roll_at_s(grid, i, j)  roll_at_index(grid, i+1, j)
#define roll_at_sw(grid, i, j) roll_at_index(grid, i+1, j-1)
#define roll_at_w(grid, i, j)  roll_at_index(grid, i, j-1)
#define roll_at_nw(grid, i, j) roll_at_index(grid, i-1, j-1)

struct input {
  int    rlen;
  int    clen;
  char **grid;
};
typedef struct input input;

input alloc_input(void);
void  free_grid(input);
void  print_grid(input);

bool can_access(input, int, int);
int  remove_roll(input, int, int);

#endif
