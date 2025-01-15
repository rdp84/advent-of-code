#ifndef DAY_11_H
#define DAY_11_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct stone {
  int64_t count;
  int64_t num;
  struct stone *next;
  struct stone *prev;
};
typedef struct stone stone;

stone *alloc_stones(char *);
void blink(stone *);
void split_stone(stone *);
void merge_stones(stone *);
int64_t count_stones(stone *);
void print_stones(stone *);
void free_stones(stone *);


#endif
