#ifndef DAY_24_H
#define DAY_24_H 1

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include <limits.h>

#define X 'x'
#define Y 'y'
#define Z 'z'

enum gate { OR, XOR, AND };
typedef enum gate gate;

struct wire {
  char *name;
  bool  isset;
  bool  value;
};
typedef struct wire wire;

struct tnode {
  wire          w;
  struct tnode *left;
  struct tnode *right;
};
typedef struct tnode tnode;

struct wirelist {
  wire             w;
  struct wirelist *next;
};
typedef struct wirelist wirelist;

struct connlist {
  char *lwire;
  gate  g;
  char *rwire;
  char *outwire;
  bool  simulated;
  struct connlist *next;
};
typedef struct connlist connlist;

struct input {
  struct tnode *wires;
  struct connlist *conns;
};
typedef struct input input;

input alloc_input(void);

tnode *add_wire(tnode *, char *);
tnode *add_wire_value(tnode *, char *, char *);
void   node_count(tnode *, int *, int *);
void   print_tree(tnode *);
void   free_tree(tnode *);

connlist *add_conn(connlist *, char *, char *, char *, char *);
void      print_conns(connlist *);
void      free_conns(connlist *);

wirelist       **alloc_table(tnode *, int *, int *);
void           add_to_table(tnode *, wirelist **, int);
wire           get_wire(wirelist **, int, char *);
void           set_wire(wirelist **, int, char *, bool);
void           print_table(wirelist **, int);
void           free_table(wirelist **, int);
unsigned long  hash(char *, int);

char *get_match(char *line, regmatch_t match);

void run_simulation(connlist *, wirelist **, int);

bool          *get_values(wirelist **, int, int, char);
unsigned long  to_long(bool *, int);
void           bit_print(unsigned long);

char *x_xor_y(connlist *, char *, char *);
char *x_xor_y_xor_carry(connlist *, char *, char *);
char *x_and_y(connlist *, char *, char *);
char *x_xor_y_and_carry(connlist *, char *, char *);
char *next_carry(connlist *, char *, char *);

#endif
