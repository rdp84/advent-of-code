#ifndef DAY_22_H
#define DAY_22_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ROUNDS  2001
#define SEQ_LEN 4
#define KEY_LEN 19

#define mix(a, b) a ^ b
#define prune(a)  a % 16777216

struct price {
  int digit;
  int delta;
};
typedef struct price price;

long step_one(long);
long step_two(long);
long step_three(long);

long **alloc_buyers(int *);
void   free_buyers(long **, int);

price **alloc_prices(long **, int);
void    print_prices(long **, price **, int);
void    free_prices(price **, int);

void reset_map(int[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN]);
bool seen_seq(price *, int[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN]);
void add_to_map(price *, int[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN]);
void print_map(int[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN]);
void find_max(int[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN]);

#endif
