#ifndef DAY_23_H
#define DAY_23_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define KEY_LEN 26

#define ctoi(c) c - 'a'
#define itoc(i) i + 'a'

struct conns_map {
  bool map[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN];
};
typedef struct conns_map conns_map_t;

struct threes {
  char *conn;
  struct threes *next;
};
typedef struct threes threes;

void init_conns_map(conns_map_t *);
void add_conn(char *, char *, conns_map_t *);
void print_conns_map(conns_map_t *);

#endif
