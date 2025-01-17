#ifndef DAY_12_H
#define DAY_12_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NORTH_CHAR(map, i, j)        i == 0 ?        '.' : *(*(map + (i - 1)) + j)
#define EAST_CHAR(map, i, j, cols)   j == cols - 1 ? '.' : *(*(map + i) + (j + 1))
#define SOUTH_CHAR(map, i, j, rows)  i == rows - 1 ? '.' : *(*(map + (i + 1)) + j)
#define WEST_CHAR(map, i, j)         j == 0 ?        '.' : *(*(map + i) + (j - 1))

struct side {
  int index;
  int size;
  int *plants;
  struct side *next;
};
typedef struct side side;

struct region {
  char plant;
  int area;
  int totalSides;
  side *northSides;
  side *eastSides;
  side *southSides;
  side *westSides;
};
typedef struct region region;

char **alloc_map(int *, int *);
region *alloc_region(char);
side *alloc_side(int, int);

void free_map(char **, int);
void free_side(side *);
void free_region(region *);

void print_map(char **, int);
void print_region(region *);
void print_sides(side *);

void walk_region(char, char **, int, int, int, int, int *);
void walk_region_2(char, char **, int, int, int, int, region *);

void add_side(side *, int, int);
void add_plant_to_side(side *, int);
int total_sides(side *);

#endif
