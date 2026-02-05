#ifndef DAY_5_H
#define DAY_5_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define min(x, y) x <= y ? x : y
#define max(x, y) x >= y ? x : y

struct range {
  long start;
  long end;
  bool merged;
};
typedef struct range range;

struct rangelist {
  range r;
  struct rangelist *next;
};
typedef struct rangelist rangelist;

rangelist *addlist(rangelist *, range);
void       freelist(rangelist *);
bool       listmember(rangelist *, long);
void       listprint(rangelist *);

bool can_merge(range, range);

#endif
