#if !defined DAY_9_H
#define DAY_9_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define shrink_rectangle(r) r.tl.col++; r.tl.row++; r.tr.col--; r.tr.row++; r.br.col--; r.br.row--; r.bl.col++; r.bl.row--

struct tile {
  int col;
  int row;
};
typedef struct tile tile;

struct pair {
  tile   rt1;
  tile   rt2;
  bool   redgreen;
  long   area;
};
typedef struct pair pair;

struct rectangle {
  tile tl;
  tile tr;
  tile br;
  tile bl;
};
typedef struct rectangle rectangle;

struct sorted {
  int min;
  int max;
};
typedef struct sorted sorted;

struct input {
  int   nrtiles;
  tile *rtiles;
  int   npairs;
  pair *pairs;
};
typedef struct input input;

input  alloc_input(void);
tile  *get_rtiles(int *);
pair  *get_pairs(tile *, int, int *);
void   free_input(input);
void   print_input(input);

rectangle make_rectangle(tile, tile);
void      print_rectangle(rectangle);
bool      rectangle_in_polygon(tile *, int, rectangle);
bool      tile_in_polygon(tile *, int, tile);
bool      on_edge(tile, tile, tile);
bool      polygon_edge_in_rectangle(tile *, int, rectangle);

sorted sort(int, int);
int    pair_cmp(const void *, const void *);

#endif
