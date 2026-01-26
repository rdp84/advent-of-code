#ifndef DAY_25_H
#define DAY_25_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define EMPTY  '.'
#define FILLED '#'

struct schematic {
  int    rows;
  int    cols;
  char **diag;
};
typedef struct schematic schematic;

struct schlist {
  schematic s;
  struct schlist *next;
};
typedef struct schlist schlist;

struct input {
  schlist *locks;
  schlist *keys;
};
typedef struct input input;

input    alloc_input(void);
schlist *add_schematic(schlist *, schematic *);
void     print_schlist(schlist *);
void     free_schlist(schlist *);

int  check_fits(schlist *, schlist *);
bool fits(schematic, schematic);

#endif
