#ifndef DAY_23_H
#define DAY_23_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define KEY_LEN 26
#define T       't'

#define ctoi(c) c - 'a'
#define itoc(i) i + 'a'

struct node {
  char   name[3];
  int    neighlen;
  char **neighbours;
};
typedef struct node node;

struct lan {
  node net[KEY_LEN][KEY_LEN];
};
typedef struct lan lan;

struct vertex_set {
  int size;
  char **vertices;
};
typedef struct vertex_set vertex_set;

struct cliques {
  int vslen;
  vertex_set *vs;
};
typedef struct cliques cliques;

void init_lan(lan *);
void print_lan(lan *);
void add_conn(char *, char *, lan *);
void free_lan(lan *);

char **alloc_pairs(node, int *);
void   print_pairs(char **, int);
void   free_pairs(char **, int);
bool   is_neighbour(lan *, char *, char *);
char **add_three(char **, int *, char *, char *, char *);
char **find_threes(lan *, int *);
void   print_threes(char **, int *);
void   free_threes(char **, int *);

vertex_set  init_vertex_set(lan *);
void        print_vertex_set(vertex_set);
void        free_vertex_set(vertex_set);
bool        in_set(char *, vertex_set);
vertex_set  uni(vertex_set, vertex_set);
vertex_set  inter(vertex_set, vertex_set);
vertex_set  minus(vertex_set, vertex_set);
vertex_set  find_pivot(lan *, vertex_set, vertex_set);
vertex_set  alloc_single_vertex(char *);
vertex_set  neighbours(char *, lan *);

void bron_kerbosch(vertex_set, vertex_set, vertex_set, lan *, cliques *);

#endif
