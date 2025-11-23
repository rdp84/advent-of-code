#ifndef DAY_16_H
#define DAY_16_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define START 'S'
#define END   'E'
#define WALL  '#'
#define FREE  '.'

#define NORTH '^'
#define EAST  '>'
#define SOUTH 'v'
#define WEST  '<'

#define TAKEN_SIZE 4

#define alloc_taken(p)    malloc((((p.steps / TAKEN_SIZE) + 1) * TAKEN_SIZE + 1) * sizeof(char))
#define realloc_taken(p)  realloc(p.taken, (((p.steps / TAKEN_SIZE) + 1) * TAKEN_SIZE + 1) * sizeof(char))
#define taken_end(p)      *(p.taken + ((p.steps / TAKEN_SIZE) + 1) * TAKEN_SIZE)

struct location {
  int score;
  int row;
  int col;
  char dir;
  struct location *next;
};
typedef struct location location;

struct path {
  int score;
  int steps;
  int row;
  int col;
  char dir;
  char *taken;
};
typedef struct path path;

struct elem {
  path p;
  struct elem *next;
};
typedef struct elem elem;

struct queue {
  int cnt;
  elem *front;
  elem *rear;
};
typedef struct queue queue;

char **alloc_map(int *, int *);
void free_map(char **, int);
void print_map(char **, int, int);

location ***alloc_seen(int, int);
int      seen_score(path, location ***);
void     free_seen(int, int, location ***);
void     print_location(location *);
void     free_location(location *);

queue *alloc_queue(void);
void  enqueue(path, queue *);
path  dequeue(queue *);
bool  empty(queue *);
void  free_queue(queue *);
void  print_queue(queue *);

void move(path, char **, queue *);
void rotate(path, queue *);
void rotate_clockwise(path, queue *);
void rotate_counter_clockwise(path, queue *);

void add_path_to_map(path, int, int, char **);

#endif
