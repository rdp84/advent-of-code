#ifndef DAY_7_H
#define DAY_7_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define START    'S'
#define SPLITTER '^'
#define EMPTY    '.'
#define BEAM     '|'

struct input {
  int    rows;
  int    cols;
  int    sindex;
  char **diag;
};
typedef struct input input;

struct bpath {
  bool splits;
  bool left;
  bool right;
  int  i;
  int  j;
};
typedef struct bpath bpath;

struct qpath {
  bool splits;
  int i;
  int j;
};
typedef struct qpath qpath;

struct splitter {
  bool visited;
  long timelines;
};
typedef struct splitter splitter;

input alloc_input(void);
void  print_input(input);
void  free_input(input);

splitter **alloc_splitters(input);
void       free_splitters(splitter **, int);

void  send_beam(input, int, int, int *);
bpath beam_path(input, int, int);

long  timelines(input, int, int, splitter **);
qpath quantum_path(input, int, int);

#endif
