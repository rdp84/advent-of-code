#ifndef DAY_1_H
#define DAY_1_H 1

#include <stdio.h>
#include <stdlib.h>

#define LEFT  'L'
#define RIGHT 'R'
#define NUMS  100

struct input {
  int    len;
  char **rot;
};
typedef struct input input;

input alloc_input(void);
void  free_input(input);

#endif
