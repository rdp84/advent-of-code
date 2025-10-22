#ifndef DAY_15_H
#define DAY_15_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FREE  '.'
#define BOX   'O'
#define WALL  '#'
#define ROBOT '@'

#define LEFT_BRACKET  '['
#define RIGHT_BRACKET ']'

#define NORTH '^'
#define EAST  '>'
#define SOUTH 'v'
#define WEST  '<'

#define char_at_index(map, i, j)   *(*(map + i) + j)

struct bracket {
  char b;
  int  iFrom;
  int  jFrom;
  int  iTo;
  int  jTo;
};
typedef struct bracket bracket;

struct elem {
  bracket b;
  struct elem *next;
};
typedef struct elem elem;

struct brackets {
  int  cnt;
  elem *top;
};
typedef struct brackets brackets;

char **alloc_map(int *, int *);
char **alloc_moves(int *, int *);
char **widen_map(char **, int, int);

void print_input(char **, int, int);
void free_input(char **, int);

void move(char **, int, int, int, int, int *, int *);
void vertical_move(char **, int, int, int, int *, brackets *);

void initialize(brackets *);
void save_bracket(char, int, int, int, int, brackets *);
bracket  pop_brackets(brackets *);
void free_brackets(brackets *);
void undo_moved_brackets(char **, brackets *);

#endif
