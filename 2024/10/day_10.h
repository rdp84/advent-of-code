#ifndef DAY_10_H
#define DAY_10_H

#include <stdio.h>
#include <stdlib.h>

struct reachable_nine {
  int row;
  int col;
  struct reachable_nine *next;
};
typedef struct reachable_nine reachable_nine;

struct trailhead {
  int row;
  int col;
  int visits;
  reachable_nine *nines;
  struct trailhead *next;
};
typedef struct trailhead trailhead;

char **alloc_map(int *, int *);
void free_map(char **, int);
void print_map(char **, int);

int sum_all_trailhead_scores(char **, int, int);
void walk_from_trailhead(char **, int, int, int, int, trailhead *);
void add_unique_reachable_nine(trailhead *, int, int);
void add_reachable_nine(trailhead *, int, int);

#endif
