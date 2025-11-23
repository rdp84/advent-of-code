#ifndef DAY_17_H
#define DAY_17_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define REGISTERS 3

#define A 'A'
#define B 'B'
#define C 'C'

#define is_literal(oprnd)  oprnd >= '0' && oprnd <= '3'
#define char_to_int(oprnd) oprnd - '0'
#define get_reg(oprnd)     ((char_to_int(oprnd)) % 4)

struct output {
  int n;
  struct output *next;
};
typedef struct output output;

struct elem {
  int64_t n;
  struct elem *next;
};
typedef struct elem elem;

struct stack {
  int cnt;
  elem *top;
};
typedef struct stack stack;

char   *alloc_program(int64_t *);
output *run_program(char *, int64_t *);
void   free_output(output *);
void   print_output(output *);

void    init_stack(stack *);
void    push(int64_t, stack *);
int64_t pop(stack *);
int64_t find_min(stack *);
bool    empty(const stack *);
void    print_stack(const stack *);
void    free_elems(stack *);

void   adv(int64_t *, char);
void   bxl(int64_t *, char);
void   bst(int64_t *, char);
void   jnz(int *, int64_t *, char);
void   bxc(int64_t *);
output *out(int64_t *, char, output *);
void   bdv(int64_t *, char);
void   cdv(int64_t *, char);

#endif
