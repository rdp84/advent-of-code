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

struct location {
  int row;
  int col;
  int rowDir;
  int colDir;
  struct location *next;
};
typedef struct location location;

struct path {
  int score;
  int row;
  int col;
  int rowDir;
  int colDir;
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

location *alloc_location(int, int, int, int);
void add_location(location *, int, int, int, int);
bool is_looping(location *, int, int, int, int);
void free_location(location *);

queue *alloc_queue(void);
void  free_queue(queue *);
void  print_queue(const queue *);
void  enqueue(path, queue *);
path  dequeue(queue *);
bool  empty(const queue *);

void  move(path, char **, queue *, location *);
void  rotate(int, int, path, char **, queue *, location *);

#endif
