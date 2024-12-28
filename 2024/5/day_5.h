#ifndef DAY_5_H
#define DAY_5_H

#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct rule {
  int page;
  struct rule *next;
};
typedef struct rule rule;

struct ordering_rules {
  int page;
  rule *rules;
  struct ordering_rules *next;
};
typedef struct ordering_rules ordering_rules;

ordering_rules *alloc_ordering_rules(FILE *);
int get_page(char *, regmatch_t);
void print_rules(ordering_rules *);
void free_ordering_rules(ordering_rules *);
void parse_updates(ordering_rules *, FILE *);
rule *get_rules(ordering_rules *, int);
void modify_updates(ordering_rules *, FILE *);

#endif
