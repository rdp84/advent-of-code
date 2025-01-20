#ifndef DAY_14_H
#define DAY_14_H 1

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define X_BOUNDARY 101
#define Y_BOUNDARY -103
#define SECONDS    100

struct robot {
  int xPos;
  int yPos;
  int xDir;
  int yDir;
  struct robot *next;
};
typedef struct robot robot;

robot *alloc_robots(void);
void free_robots(robot *);
void find_end_pos(robot *, int);

int **alloc_map(void);
void draw_map(int **, robot *);
void free_map(int **);
void print_map(int **);

void read_line(char *, int *, int *, int *, int *);
int get_user_input(char []);
int find_cluster(int **);

#endif
