#ifndef DAY_8_H
#define DAY_8_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define inside_x_bound(x, xBound) x >= 0 && x < xBound
#define inside_y_bound(y, yBound) y <= 0 && y > yBound
#define add_antinode(antinodes, x, y) *(*(antinodes + -y) + x) = 'A'
#define is_antinode(antinodes, i, j) *(*(antinodes + i) + j) == 'A'
#define add_all_inline_antinodes(antinodes, x, xBound, y, yBound, vector) \
  while (inside_x_bound(x, xBound) && inside_y_bound(y, yBound)) { \
    add_antinode(antinodes, x, y); \
    x += vector.x;                 \
    y += vector.y;                 \
  }                                \

struct coord {
  int x;
  int y;
  struct coord *next;
};
typedef struct coord coord;

struct antenna {
  char c;
  struct coord *coords;
  struct antenna *next;
};
typedef struct antenna antenna;

antenna *alloc_antennas(int *, int *);
antenna *alloc_antenna(char, int, int);
coord *alloc_coord(int, int);
char **alloc_antinodes(int, int);
void free_antennas(antenna *);
int free_antinodes(char **, int, int);
void print_antennas(antenna *);
coord vector_a_b(coord *, coord *);
void count_unique_antinodes_part_1(antenna *, int, int);
void count_unique_antinodes_part_2(antenna *, int, int);

#endif
