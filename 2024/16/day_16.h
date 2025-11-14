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

#define can_move_north(map, i, j) *(*(map + (i - 1)) + j) != WALL
#define can_move_east(map, i, j)  *(*(map + i) + (j + 1)) != WALL
#define can_move_south(map, i, j) *(*(map + (i + 1)) + j) != WALL
#define can_move_west(map, i, j)  *(*(map + i) + (j - 1)) != WALL

struct location {
  int row;
  int col;
  int rowDir;
  int colDir;
  struct location *next;
};
typedef struct location location;

struct elem {
  int score;
  int row;
  int col;
  int rowDir;
  int colDir;
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

location *alloc_location(int, int, int, int, int);
void add_location(location *, int, int, int, int);
bool is_looping(location *, int, int, int, int);
void free_location(location *);

queue *alloc_queue(void);
void  free_queue(queue *);
void  enqueue(int, int, int, int, int, queue *);
data  dequeue(queue *);
data  front(const queue *);
bool  empty(const queue *);

#endif
