#include "day_5.h"

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
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *tmp;
  int cnt;
  rangelist *l;
  range r;

  ifp = fopen("input.txt", "r");
  linecap = 0;
  line = NULL;
  l = NULL;
  r.merged = false;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    tmp = line;
    r.start = atol(strsep(&line, "-"));
    r.end = atol(line);
    line = tmp;
    l = addlist(l, r);
  }
  
  free(line);
  line = NULL;
  cnt = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 1)
    if (listmember(l, atol(line)))
      cnt++;

  free(line);
  fclose(ifp);
  freelist(l);
  printf("cnt: %d\n", cnt);
}

void part_2(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *tmp;
  bool hasmerged;
  int i, j, rlen;
  long cnt;
  range r, *ri, *rj, *ranges;

  ifp = fopen("input.txt", "r");
  linecap = 0;
  line = NULL;

  rlen = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 1)
    rlen++;
  ranges = malloc(rlen * sizeof(range));

  rewind(ifp);
  r.merged = false;
  i = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    tmp = line;
    r.start = atol(strsep(&line, "-"));
    r.end = atol(line);
    line = tmp;
    *(ranges + i++) = r;
  }
  free(line);
  fclose(ifp);

  for (i = 0; i < rlen - 1; i++) {
    hasmerged = false;
    ri = ranges + i;
    for (j = i + 1; j < rlen; j++) {
      rj = ranges + j;
      if (can_merge(*ri, *rj)) {
        ri->start = min(ri->start, rj->start);
        ri->end = max(ri->end, rj->end);
        hasmerged = rj->merged = true;
      }
    }
    if (hasmerged)
      i--;
  }

  cnt = 0;
  for (i = 0; i < rlen; i++) {
    ri = ranges + i;
    if (!ri->merged)
      cnt += (ri->end - ri->start) + 1;
  }
  printf("cnt: %ld\n", cnt);
  free(ranges);
}

rangelist *addlist(rangelist *p, range r)
{
  if (p == NULL) {
    p = malloc(sizeof(rangelist));
    p->r = r;
    p->next = NULL;
  } else
    p->next = addlist(p->next, r);

  return p;
}

void freelist(rangelist *p)
{
  if (p != NULL) {
    freelist(p->next);
    free(p);
  }
}

bool listmember(rangelist *p, long id)
{
  if (p == NULL)
    return false;
  else if (id >= p->r.start && id <= p->r.end)
    return true;
  else
    return listmember(p->next, id);
}

void listprint(rangelist *p)
{
  if (p == NULL)
    printf("NULL\n");
  else {
    printf("start: %ld, end: %ld\n", p->r.start, p->r.end);
    listprint(p->next);
  }
}

bool can_merge(range r1, range r2)
{
  if (r1.merged || r2.merged)
    return false;
  else if (r1.start < r2.start - 1)
    if (r1.end < r2.start - 1)
      return false;
    else
      return true;
  else if (r1.start > r2.end + 1)
    return false;
  else
    return true;
}
