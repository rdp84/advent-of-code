#if !defined DAY_10_H
#define DAY_10_H 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>

struct diag {
  int  len;
  int  ilights;
};
typedef struct diag diag;

struct btns {
  int  len;
  int *btns;
};
typedef struct btns btns;

struct jolt {
  int len;
  int *reqs;
};
typedef struct jolt jolt;

diag get_diag(char **);
void print_diag(diag);

btns alloc_btns(char **, int);
void print_btns(btns);
void free_btns(btns);

jolt alloc_jolt(char **);
void print_jolt(jolt);
void free_jolt(jolt);

void bit_print(int);

#endif
