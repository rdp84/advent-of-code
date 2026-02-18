#include "day_8.h"

void part_1(void);
void part_2(void);

int main(void)
{
  part_1();
  part_2();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  input in;
  circuitdll *cirs;
  int i;
  long ans;

  in = alloc_input();

  cirs = NULL;
  for (i = 0; i < 1000; i++)
    cirs = connect(in, i, cirs);
  ans = three_largest(cirs);
  printf("ans: %ld\n", ans);

  free_input(in, cirs);
}

void part_2(void)
{
  input in;
  box b1, b2;
  circuitdll *cirs;
  bool oneCircuit;
  int i;

  in = alloc_input();

  oneCircuit = false;
  cirs = NULL;
  for (i = 0; i < in.npairs && !oneCircuit; i++) {
    cirs = connect(in, i, cirs);
    oneCircuit = has_one_circuit(in.hashtab, in.nkeys);
  }
  b1 = (*(in.pairs + --i)).b1;
  b2 = (*(in.pairs + i)).b2;
  printf("b1: (x:%ld, y:%ld, x:%ld), b2: (x:%ld, y:%ld, z:%ld), ans: %ld\n", b1.x, b1.y, b1.z, b2.x, b2.y, b2.z, b1.x * b2.x);

  free_input(in, cirs);
}

input alloc_input(void)
{
  FILE *ifp;
  int nboxes, npairs;
  box *boxes;
  input retValue;

  ifp = fopen("input.txt", "r");
  nboxes = npairs = 0;
  boxes = get_boxes(ifp, &nboxes);
  retValue.pairs = get_pairs(boxes, nboxes, &npairs);
  retValue.npairs = npairs;
  retValue.nkeys = nboxes;
  retValue.hashtab = alloc_table(boxes, nboxes);
  
  fclose(ifp);
  free(boxes);
  return retValue;
}

box *get_boxes(FILE *ifp, int *nboxes)
{
  size_t linecap;
  char *line, *start, delim[] = ",";
  int i;
  box *b, *retValue;

  linecap = 0;
  line = NULL;
  *nboxes = 0;
  while (getline(&line, &linecap, ifp) > 0)
    (*nboxes)++;
  retValue = malloc(*nboxes * sizeof(box));
  rewind(ifp);

  i = 0;
  while (getline(&line, &linecap, ifp) > 0) {
    start = line;

    b = &(*(retValue + i++));
    b->x = atoi(strsep(&line, delim));
    b->y = atoi(strsep(&line, delim));
    b->z = atoi(strsep(&line, delim));

    line = start;
  }

  free(line);
  return retValue;
}

pair *get_pairs(box *boxes, int nboxes, int *npairs)
{
  pair p, *retValue;
  int i, j;

  *npairs = (nboxes * (nboxes - 1)) / 2;
  retValue = malloc(*npairs * sizeof(pair));

  for (i = 0; i < nboxes - 1; i++) {
    p.b1 = *(boxes + i);
    for (j = i + 1; j < nboxes; j++) {
      p.b2 = *(boxes + j);
      p.dist = dist(p.b1, p.b2);
      *retValue++ = p;
    }
  }
  retValue -= *npairs;
  qsort(retValue, *npairs, sizeof(pair), pair_cmp);

  return retValue;
}

void free_input(input in, circuitdll *cirs)
{
  free(in.pairs);
  free_table(in.hashtab, in.nkeys);
  free_circuit(cirs);
}

long dist(box b1, box b2)
{
  long x, y, z;

  x = labs(b1.x - b2.x);
  x *= x;
  y = labs(b1.y - b2.y);
  y *= y;
  z = labs(b1.z - b2.z);
  z *= z;

  return sqrtl(x + y + z);
}

int pair_cmp(const void *va, const void *vb)
{
  const pair *a, *b;
  long diff;

  a = va;
  b = vb;
  diff = a->dist - b->dist;

  return ((diff >= 0) ? ((diff > 0) ? 1 : 0) : -1);
}

circuitdll *connect(input in, int i, circuitdll *cirs)
{
  box b1, b2;
  boxlist *bp;
  circuitdll *c1, *c2;

  b1 = (*(in.pairs + i)).b1;
  b2 = (*(in.pairs + i)).b2;
  bp = lookup(in.hashtab, in.nkeys, b1);
  c1 = bp != NULL ? bp->c : NULL;
  bp = lookup(in.hashtab, in.nkeys, b2);
  c2 = bp != NULL ? bp->c : NULL;

  if (c1 == NULL)
    if (c2 == NULL)
      cirs = add_circuit(b1, b2, cirs, in.hashtab, in.nkeys);
    else
      add_box_to_circuit(b1, c2, in.hashtab, in.nkeys);
  else
    if (c2 == NULL)
      add_box_to_circuit(b2, c1, in.hashtab, in.nkeys);
    else if (c1 != c2)
      cirs = merge_circuit(c1, c2, in.hashtab, in.nkeys);

  return cirs;
}

circuitdll *add_circuit(box b1, box b2, circuitdll *head, boxlist **hashtab, int nkeys)
{
  circuitdll *p, *np;

  np = malloc(sizeof(circuitdll));
  np->bnum = 2;
  np->boxes = malloc(NBOXES * sizeof(box));
  *(np->boxes) = b1;
  *(np->boxes + 1) = b2;
  np->next = NULL;

  install(hashtab, nkeys, b1, np);
  install(hashtab, nkeys, b2, np);

  if ((p = head) != NULL) {
    for (; p->next != NULL; p = p->next) ;
    p->next = np;
    np->prev = p;
    return head;
  }
  else {
    np->prev = NULL;
    return np;
  }
}

void add_box_to_circuit(box b, circuitdll *c, boxlist **hashtab, int nkeys)
{
  if ((c->bnum % NBOXES) == 0)
    c->boxes = realloc(c->boxes, ((c->bnum / NBOXES) + 1) * NBOXES * sizeof(box));

  *(c->boxes + c->bnum) = b;
  c->bnum++;
  install(hashtab, nkeys, b, c);
}

circuitdll *merge_circuit(circuitdll *c1, circuitdll *c2, boxlist **hashtab, int nkeys)
{
  int i;
  circuitdll *to, *from;

  if (c1->bnum > c2->bnum) {
    to = c1;
    from = c2;
  } else {
    to = c2;
    from = c1;
  }

  for (i = 0; i < from->bnum; i++)
    add_box_to_circuit(*(from->boxes + i), to, hashtab, nkeys);

  if (from->next != NULL)
    from->next->prev = from->prev;
  if (from->prev != NULL)
    from->prev->next = from->next;

  free(from->boxes);
  free(from);

  for (; to->prev != NULL; to = to->prev) ;
  return to;
}

void print_circuit(circuitdll *head)
{
  int i;
  box b;

  for (; head != NULL; head = head->next) {
    printf("Circuit with %d boxes:\n", head->bnum);
    for (i = 0; i < head->bnum - 1; i++) {
      b = *(head->boxes + i);
      printf("(x:%ld, y:%ld, z:%ld), ", b.x, b.y, b.z);
    }
    b = *(head->boxes + i);
    printf("(x:%ld, y:%ld, z:%ld)\n", b.x, b.y, b.z);
  }

}

void free_circuit(circuitdll *head)
{
  circuitdll *toFree;

  while (head != NULL) {
    toFree = head;
    head = head->next;
    free(toFree->boxes);
    free(toFree);
  }
}

long three_largest(circuitdll *head)
{
  int n, l[3];

  l[0] = l[1] = l[2] = INT_MIN;
  for (; head != NULL; head = head->next) {
    n = head->bnum;
    if (n > l[0]) {
      l[2] = l[1];
      l[1] = l[0];
      l[0] = n;
    } else if (n > l[1]) {
      l[2] = l[1];
      l[1] = n;
    } else if (n > l[2])
      l[2] = n;
  }

  return l[0] * l[1] * l[2];  
}
