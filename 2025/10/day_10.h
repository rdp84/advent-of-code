#if !defined DAY_10_H
#define DAY_10_H 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct diag {
  int  len;
  int *ilights;
};
typedef struct diag diag;

struct btn {
  int  len;
  int *wiring;
};
typedef struct btn btn;

struct btnlist {
  btn b;
  struct btnlist *next;
};
typedef struct btnlist btnlist;

struct jolt {
  int len;
  int *reqs;
};
typedef struct jolt jolt;

diag alloc_diag(char **);
void print_diag(diag);
void free_diag(diag);

btnlist *alloc_btns(char **);
void     print_btns(btnlist *);
void     free_btns(btnlist *);

jolt alloc_jolt(char **);
void print_jolt(jolt);
void free_jolt(jolt);

#endif
