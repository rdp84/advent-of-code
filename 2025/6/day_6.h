#ifndef DAY_6_H
#define DAY_6_H 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct input {
  int vlen;
  int hlen;
  int **nums;
};
typedef struct input input;

FILE *open_input(void);
void  close_input(FILE *);
int   get_vertical_len(FILE *);
int   get_horizontal_len(FILE *);
input alloc_input(FILE *, int, int);

#endif
