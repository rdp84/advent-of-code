#include "day_25.h"

void part_1(void);

int main(void)
{
  part_1();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  int ans;
  input in;
  in = alloc_input();

  ans = check_fits(in.keys, in.locks);
  printf("total fits: %d\n", ans);
  
  free_schlist(in.locks);
  free_schlist(in.keys);
}

input alloc_input(void)
{
  char *line;
  bool islock;
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  schematic toadd;
  input retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  retValue.keys = retValue.locks = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toadd.diag = malloc(sizeof(char *));
    *(line + linelen - 1) = '\0';
    *(toadd.diag) = line;
    toadd.rows = 1;
    toadd.cols = linelen - 1;
    islock = *line == FILLED ? true : false;
    line = NULL;

    while ((linelen = getline(&line, &linecap, ifp)) > 1) {
      toadd.diag = realloc(toadd.diag, ++toadd.rows * sizeof(char *));
      *(line + linelen - 1) = '\0';
      *(toadd.diag + toadd.rows - 1) = line; 
      line = NULL;
    }
    if (islock)
      retValue.locks = add_schematic(retValue.locks, &toadd);
    else
      retValue.keys = add_schematic(retValue.keys, &toadd);

    free(line);
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

schlist *add_schematic(schlist *p, schematic *s)
{
  if (p == NULL) {
    p = malloc(sizeof(schlist));
    p->s = *s;
    p->next = NULL;
  }
  else
    p->next = add_schematic(p->next, s);

  return p;
}

void print_schlist(schlist *p)
{
  int i;
  while (p != NULL) {
    printf("schematic has rows: %d, cols: %d\n", p->s.rows, p->s.cols);
    for (i = 0; i < p->s.rows; i++)
      printf("%s\n", *(p->s.diag + i));
    p = p->next;
  }
}

void free_schlist(schlist *p)
{
  int i;
  schlist *tofree;

  while (p != NULL) {
    tofree = p;
    for (i = 0; i < p->s.rows; i++)
      free(*(p->s.diag + i));
    free(p->s.diag);
    p = p->next;
    free(tofree);
  }
}

int check_fits(schlist *keys, schlist *locks)
{
  int retValue;
  schlist *headl;

  headl = locks;
  retValue = 0;
  while (keys != NULL) {
    while (locks != NULL) {
      if (fits(keys->s, locks->s))
        retValue++;
      locks = locks->next;
    }
    locks = headl;
    keys = keys->next;
  }
  return retValue;
}


bool fits(schematic key, schematic lock)
{
  char k, l;
  int i, j;
  
  if (key.rows == lock.rows && key.cols == lock.cols) {
    for (i = 0; i < key.cols; i++) {
      for (j = 0; j < key.rows; j++) {
        k = *(*(key.diag + j) + i);
        l = *(*(lock.diag + j) + i);
        if (k == FILLED && l == FILLED)
          return false;
      }
    }
  }
  else
    return false;

  return true;
}
