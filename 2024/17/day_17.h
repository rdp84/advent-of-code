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


char *alloc_program(int *);
void free_output(output *);
void print_output(output *);

void   adv(int *, char);
void   bxl(int *, char);
void   bst(int *, char);
void   jnz(int *, int *, char);
void   bxc(int *);
output *out(int *, char, output *);
void   bdv(int *, char);
void   cdv(int *, char);

#endif
