#ifndef DAY_21_H
#define DAY_21_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define UP    '^'
#define DOWN  'v'
#define RIGHT '>'
#define LEFT  '<'

#define ZERO  '0'
#define ONE   '1'
#define TWO   '2'
#define THREE '3'
#define FOUR  '4'
#define FIVE  '5'
#define SIX   '6'
#define SEVEN '7'
#define EIGHT '8'
#define NINE  '9'

#define SPACE    ' '
#define ACTIVATE 'A'

#define NUMERICAL_PAD_ROWS 4
#define NUMERICAL_PAD_COLS 3

#define DIRECTION_PAD_ROWS 2
#define DIRECTION_PAD_COLS 3

struct location {
  int  score;
  int  row;
  int  col;
  char dir;
  struct location *next;
};
typedef struct location location;

struct path {
  int   score;
  int   row;
  int   col;
  char  dir;
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

struct best {
  int cnt;
  char **paths;
};
typedef struct best best;

struct entry {
  char *code;
  long  cost;
  struct entry *next;
};
typedef struct entry entry;

queue *alloc_queue(void);
void   enqueue(path, queue *);
path   dequeue(queue *);
bool   empty(queue *);
void   free_queue(queue *);
void   print_queue(queue *);

location ***alloc_seen(int, int);
int         seen_score(location ***, path);
void        free_seen(location ***, int, int);
void        print_location(location *);
void        free_location(location *);

void rotate(path, queue *);
void rotate_one_eighty(path, queue *);
void rotate_ninety_clockwise(path, queue *);
void rotate_ninety_counter_clockwise(path, queue *);
void move(path, queue *);

char   **alloc_numpad(void);
char   **alloc_dirpad(void);
void     free_pad(char **, int);
void     print_pad(char **, int);

best ****alloc_pad_paths(int, int);
void     init_pad_paths(char **, int, int, best ****);
void     print_pad_paths(best ****, int, int);
void     free_pad_paths(best ****, int, int);

entry **alloc_cache(int);
void    print_cache(entry **, int);
long    get_cost(entry **, int, char *);
void    add_cost(entry **, int, char *, long);
void    free_cache(entry **, int);

char **alloc_start_codes(int *);
void   print_start_codes(char **, int);
void   free_start_codes(char **, int);
char  *prepend_activate(char *);
long   numeric_code(char *);
void   find_num_index(char, int *, int *);
void   find_dir_index(char, int *, int *);

long find_cost(int);
long find_numpad_cost(char *, int, best ****, best ****, entry **);
long find_dirpad_cost(char *, int, best ****, entry **);
long min_cost(long *, int);

#endif
