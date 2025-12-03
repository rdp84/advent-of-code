#ifndef DAY_18_H
#define DAY_18_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#define X_SIZE   71
#define Y_SIZE   71
#define NANO_SEC 1024

#define SAFE      '.'
#define CORRUPTED '#'

#define UP    '^'
#define DOWN  'v'
#define LEFT  '<'
#define RIGHT '>'

#define TAKEN_SIZE 32

#define alloc_size(p)     (((p.takenPos / TAKEN_SIZE) + 1) * TAKEN_SIZE + 1)
#define malloc_taken(p)   malloc(alloc_size(p) * sizeof(char))
#define realloc_taken(p)  realloc(p.taken, alloc_size(p) * sizeof(char))
#define taken_end(p)      *(p.taken + (((p.takenPos / TAKEN_SIZE) + 1) * TAKEN_SIZE))
#define in_bounds(x, y)   x >= 0 && x < X_SIZE && y >= 0 && y < Y_SIZE

struct coord {
  int x;
  int y;
};
typedef struct coord coord;

struct location {
  int  score;
  int  x;
  int  y;
  char dir;
  struct location *next;
};
typedef struct location location;

struct path {
  int  score;
  int  x;
  int  y;
  char dir;
  int  takenPos;
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

coord *alloc_coords(int *);
char **alloc_mem_space(void);
void   add_bytes(coord *, char **, int);
void   clear_bytes(char **);
void   free_coords(coord *);
void   free_mem_space(char **);
void   print_coords(coord *, int);
void   print_mem_space(char **);

location ***alloc_seen(void);
int         seen_score(location ***, path);
void        free_seen(location***);
void        print_location(location *);
void        free_location(location *);

queue *alloc_queue(void);
void   enqueue(path, queue *);
path   dequeue(queue *);
bool   empty(queue *);
void   free_queue(queue *);
void   print_queue(queue *);

void move(path, char **, queue *);
void rotate(path, queue *);
void rotate_one_eighty(path, queue *);
void rotate_ninety_clockwise(path, queue *);
void rotate_ninety_counter_clockwise(path, queue *);

void add_taken_to_mem(char *, char **);

#endif
