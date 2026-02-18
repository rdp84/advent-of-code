#include "day_8.h"

boxlist **alloc_table(box *boxes, int nboxes)
{
  int i;
  boxlist **retValue;

  retValue = malloc(nboxes * sizeof(boxlist *));
  for (i = 0; i < nboxes; i++)
    *(retValue + i) = NULL;
  return retValue;
}

boxlist *lookup(boxlist **hashtab, int nkeys, box b)
{
  int i;
  boxlist *bp;

  i = hash(b, nkeys);
  for (bp = *(hashtab + i); bp != NULL; bp = bp->next)
    if (b.x == bp->b.x && b.y == bp->b.y && b.z == bp->b.z)
      return bp;
  return NULL;
}

void install(boxlist **hashtab, int nkeys, box b, circuitdll *c)
{
  int i;
  boxlist *bp;

  if ((bp = lookup(hashtab, nkeys, b)) == NULL) {
    bp = malloc(sizeof(boxlist));
    bp->b = b;
    bp->c = c;
    i = hash(b, nkeys);
    bp->next = *(hashtab + i);
    *(hashtab + i) = bp;
  } else
    bp->c = c;  
}

bool has_one_circuit(boxlist **hashtab, int nkeys)
{
  int i;

  for (i = 0; i < nkeys && *(hashtab + i) == NULL; i++) ;
  
  return (*(hashtab + i))->c != NULL && (*(hashtab + i))->c->bnum == nkeys;
}

unsigned long hash(box b, int nkeys)
{
  unsigned long retVal;

  retVal = 5381;
  retVal = hashval(b.x, retVal);
  retVal = hashval(b.y, retVal);
  retVal = hashval(b.z, retVal);

  return retVal % nkeys;
}

void free_table(boxlist **hashtab, int nkeys)
{
  int i;
  boxlist *p, *toFree;

  for (i = 0; i < nkeys; i++) {
    p = *(hashtab + i);
    while (p != NULL) {
      toFree = p;
      p = p->next;
      free(toFree);
    }
  }
  free(hashtab);
}

void print_table(boxlist **hashtab, int nkeys)
{
  int i, bnum;
  box b;
  boxlist *p;

  for (i = 0; i < nkeys; i++) {
    p = *(hashtab + i);
    printf("hashval %d has boxes%s", i, p == NULL ? " NULL\n" : ":\n");
    while (p != NULL) {
      b = p->b;
      bnum = p->c == NULL ? 0 : p->c->bnum;
      printf("{(x:%ld, y:%ld, x:%ld), %p, %d%s", b.x, b.y, b.z, (void *) p->c, bnum, p->next == NULL ? "}\n" : "}, ");
      p = p->next;
    }
  }
}
