#ifndef DAY_19_H
#define DAY_19_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct towel {
  int   patlen;
  char *pat;
};
typedef struct towel towel;

towel  *alloc_towels(int *);
char  **alloc_designs(int *);
char   *alloc_pat(char *, int);
void    print_towels(towel *, int);
void    print_designs(char **, int);
void    free_designs(char **, int);

long make_design(char *, towel *, int, long *);

#endif
