#ifndef DAY_6_H
#define DAY_6_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct location {
  int  row;
  int  col;
  char dir;
  struct location *next;
};
typedef struct location location;

struct visited_locations {
  int visits;
  struct location *locations;
};
typedef struct visited_locations visited_locations;

#define NORTH          '^'
#define EAST           '>'
#define SOUTH          'v'
#define WEST           '<'
#define OBSTACLE       '#'
#define LOOPED         'L' 
#define VISITED        'X'
#define FREE           '.'

#define current_index(i, j, map) *(*(map + i) + j)
#define north_index(i, j, map)   *(*(map + (i - 1)) + j)
#define east_index(i, j, map)    *(*(map + i) + (j + 1))
#define south_index(i, j, map)   *(*(map + (i + 1)) + j)
#define west_index(i, j, map)    *(*(map + i) + (j - 1))

#define obstacle_current(i, j, map) (current_index(i, j, map) == OBSTACLE)
#define obstacle_north(i, j, map) (north_index(i, j, map) == OBSTACLE)
#define obstacle_east(i, j, map)  (east_index(i, j, map) == OBSTACLE)
#define obstacle_south(i, j, map) (south_index(i, j, map) == OBSTACLE)
#define obstacle_west(i, j, map)  (west_index(i, j, map) == OBSTACLE)

#define add_obstacle(i, j, map) current_index(i, j, map) = OBSTACLE
#define set_free(i, j, map)     current_index(i, j, map) = FREE

#define mark_visited(i, j, map, count)  \
  if ((*(*(map + i) + j)) != VISITED) { \
    count++;                            \
    *(*(map + i) + j) = VISITED;        \
  }                                     \

char **alloc_map(int *, int *);
void find_start(char **, int, int, char *, int *, int *);
void walk_map(char **, int, int);
void find_loops(char **, int, int);
void free_map(char **, int);
void print_map(char **, int);

visited_locations *alloc_visited_locations(void);
void add_visited_location(visited_locations *, int, int, char);
bool check_for_loop(visited_locations *, int, int, char);
void reset_visited_locations(visited_locations *);
void free_visited_locations(visited_locations *);
void print_visited_locations(visited_locations *);

#endif
