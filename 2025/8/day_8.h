#ifndef DAY_8_H
#define DAY_8_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>

#define NBOXES 8

#define hashval(n, h) n + 33 * h 

struct box {
  long x;
  long y;
  long z;
};
typedef struct box box;

struct circuitdll {
  int  bnum;
  box *boxes;
  struct circuitdll *next;
  struct circuitdll *prev;
};
typedef struct circuitdll circuitdll;

struct boxlist {
  box b;
  circuitdll *c;
  struct boxlist *next;
};
typedef struct boxlist boxlist;

struct pair {
  box  b1;
  box  b2;
  long dist;
};
typedef struct pair pair;

struct input {
  int       npairs;
  pair     *pairs;
  int       nkeys;
  boxlist **hashtab;
};
typedef struct input input;

input alloc_input(void);
box  *get_boxes(FILE *, int *);
pair *get_pairs(box *, int, int *);
void  print_input(input);
void  free_input(input, circuitdll *);

long dist(box, box);
int  pair_cmp(const void *, const void *);

circuitdll *connect(input, int, circuitdll *);
circuitdll *add_circuit(box, box, circuitdll *, boxlist **, int);
void        add_box_to_circuit(box, circuitdll *, boxlist **, int);
circuitdll *merge_circuit(circuitdll *, circuitdll *, boxlist **, int);
long        three_largest(circuitdll *);
void        print_circuit(circuitdll *);
void        free_circuit(circuitdll *);

boxlist       **alloc_table(box *, int);
boxlist        *lookup(boxlist **, int, box);
void            install(boxlist **, int, box, circuitdll *);
bool            has_one_circuit(boxlist **, int);
void            free_table(boxlist **, int);
void            print_table(boxlist **, int);
unsigned long   hash(box, int);

#endif
