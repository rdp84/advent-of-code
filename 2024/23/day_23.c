#include "day_23.h"

void part_1(void);
void part_2(void);

int main(void)
{
  /* part_1(); */
  part_2();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  char **thrs;
  int threeslen;
  lan l;

  init_lan(&l);

  threeslen = 0;
  thrs = find_threes(&l, &threeslen);
  printf("total number of threes: %d\n", threeslen);

  free_threes(thrs, &threeslen);
  free_lan(&l);
}

void part_2(void)
{
  int i, max, imax;
  lan l;
  vertex_set r, p, x, c;
  cliques res;

  init_lan(&l);
  p = init_vertex_set(&l);
  r.size = x.size = 0;
  r.vertices = x.vertices = NULL;

  res.vslen = 0;
  res.vs = NULL;

  bron_kerbosch(r, p, x, &l, &res);

  max = INT32_MIN;
  for (i = 0; i < res.vslen; i++) {
    c = *(res.vs + i);
    if (c.size > max) {
      max = c.size;
      imax = i;
    }
  }

  printf("maximal clique found\n");
  print_vertex_set(*(res.vs + imax));
  for (i = 0; i < res.vslen; i++)
    free_vertex_set(*(res.vs + i));

  free(res.vs);
  free_lan(&l);
}

void init_lan(lan *l)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *tofree, *from, *to;
  int i, j;

  for (i = 0; i < KEY_LEN; i++) {
    for (j = 0; j < KEY_LEN; j++) {
      l->net[i][j].name[0] = itoc(i);
      l->net[i][j].name[1] = itoc(j);
      l->net[i][j].name[2] = '\0';
      l->net[i][j].neighbours = NULL;
      l->net[i][j].neighlen = 0;
    }
  }

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    tofree = line;
    from = strsep(&line, "-");
    to = line;
    *(from + 2) = *(to + 2) = '\0';
    add_conn(from, to, l);

    free(tofree);
    line = NULL;
  }

  fclose(ifp);
}

void print_lan(lan *l)
{
  int i, j, k;
  node n;

  for (i = 0; i < KEY_LEN; i++) {
    for (j = 0; j < KEY_LEN; j++) {
      n = l->net[i][j];
      printf("node %s has %d neighbours%s", n.name, n.neighlen, n.neighlen != 0 ? ":\n" : "\n");
      for (k = 0; k < n.neighlen; k++)
        printf("%s\n", *(n.neighbours + k));
    }
  }
}

void add_conn(char *from, char *to, lan *l)
{
  int i, j;
  node *n;

  i = ctoi(*from);
  j = ctoi(*(from + 1));
  n = &(l->net[i][j]);
  n->neighbours = realloc(n->neighbours, (n->neighlen + 1) * sizeof(char *));
  *(n->neighbours + n->neighlen++) = strdup(to);

  i = ctoi(*to);
  j = ctoi(*(to + 1));
  n = &(l->net[i][j]);
  n->neighbours = realloc(n->neighbours, (n->neighlen + 1) * sizeof(char *));
  *(n->neighbours + n->neighlen++) = strdup(from);
}

void free_lan(lan *l)
{
  int i, j;

  for (i = 0; i < KEY_LEN; i++)
    for (j = 0; j < KEY_LEN; j++)
      free(l->net[i][j].neighbours);
}

char **alloc_pairs(node n, int *retLen)
{
  char *bneigh, *cneigh, **retValue;
  int i, j, k;

  if (n.neighlen <= 1) {
    *retLen = 0;
    retValue = NULL;
  }
  else {
    *retLen = (n.neighlen * (n.neighlen - 1)) / 2;
    retValue = malloc((*retLen) * sizeof(char *));

    k = 0;
    for (i = 0; i < n.neighlen; i++) {
      bneigh = *(n.neighbours + i);
      for (j = i + 1; j < n.neighlen; j++) {
        cneigh = *(n.neighbours + j);
        *(retValue + k) = malloc(6 * sizeof(char));
        *(*(retValue + k)) = '\0';
        (void) strcat(*(retValue + k), bneigh);
        *(*(retValue + k) + 2) = ',';
        *(*(retValue + k) + 3) = '\0';
        (void) strcat(*(retValue + k), cneigh);
        k++;
      }
    }
  }
  return retValue;
}

void print_pairs(char **pairs, int len)
{
  int i;

  for (i = 0; i < len; i++)
    printf("(%s) ", *(pairs + i));
  printf("\n");
}

void free_pairs(char **pairs, int len)
{
  int i;

  for (i = 0; i < len; i++)
    free(*(pairs + i));
  free(pairs);
}

bool is_neighbour(lan *l, char *a, char *b)
{
  int i, j;
  node n;

  i = ctoi(*a);
  j = ctoi(*(a + 1));
  n = l->net[i][j];

  for (i = 0; i < n.neighlen; i++)
    if (strcmp(*(n.neighbours + i), b) == 0)
      return true;

  return false;
}

char **add_three(char **thrs, int *len, char *start, char *b, char *c)
{
  char *test, *thrstr;
  int i;

  if (*start != T && *b != T && *c != T)
    return thrs;

  for (i = 0; i < *len; i++) {
    test = *(thrs + i);
    if (strstr(test, start) != NULL &&
        strstr(test, b) != NULL &&
        strstr(test, c))
      return thrs;
  }

  thrs = realloc(thrs, (*len + 1) * sizeof(char *));

  thrstr = malloc(9 * sizeof(char));
  *thrstr = '\0';
  (void) strcat(thrstr, start);
  *(thrstr + 2) = ',';
  *(thrstr + 3) = '\0';
  (void) strcat(thrstr, b);
  *(thrstr + 5) = ',';
  *(thrstr + 6) = '\0';
  (void) strcat(thrstr, c);

  *(thrs + *len) = thrstr;
  (*len)++;

  return thrs;
}

char **find_threes(lan *l, int *len)
{
  char **retValue, **pairs, *b, *c, *tmp;
  int i, j, k, plen;
  node n;

  retValue = NULL;
  *len = 0;
  for (i = 0; i < KEY_LEN; i++) {
    for (j = 0; j < KEY_LEN; j++) {
      n = l->net[i][j];
      plen = 0;
      pairs = alloc_pairs(n, &plen);

      for (k = 0; k < plen; k++) {
        tmp = *(pairs + k);
        b = strsep(&(*(pairs + k)), ",");
        c = *(pairs + k);
        *(pairs + k) = tmp;

        if (is_neighbour(l, b, c))
          retValue = add_three(retValue, len, n.name, b, c);
      }
      free_pairs(pairs, plen);
    }
  }
  return retValue;
}

void print_threes(char **thrs, int *len)
{
  int i;

  printf("total number of threes: %d\n", *len);
  for (i = 0; i < *len; i++)
    printf("%s\n", *(thrs + i));
}

void free_threes(char **thrs, int *len)
{
  int i;

  for (i = 0; i < *len; i++)
    free(*(thrs + i));
  free(thrs);
}

vertex_set init_vertex_set(lan *l)
{
  int i, j;
  node n;
  vertex_set retValue;

  retValue.size = 0;
  retValue.vertices = NULL;

  for (i = 0; i < KEY_LEN; i++) {
    for (j = 0; j < KEY_LEN; j++) {
      n = l->net[i][j];
      if (n.neighlen > 0) {
        retValue.size++;
        retValue.vertices = realloc(retValue.vertices, (retValue.size * sizeof(char *)));
        *(retValue.vertices + (retValue.size - 1)) = strdup(n.name);
      }
    }
  }
  return retValue;
}

void print_vertex_set(vertex_set s)
{
  int i;

  printf("vertex set has %d vertices%s", s.size, s.size == 0 ? "\n" : ":\n");
  for (i = 0; i < s.size; i++)
    printf("%s%s", *(s.vertices + i), i < s.size - 1 ? "," : "\n");
}

void free_vertex_set(vertex_set s)
{
  int i;

  for (i = 0; i < s.size; i++)
    free(*(s.vertices + i));
  free(s.vertices);
}

bool in_set(char *vertex, vertex_set s)
{
  int i;

  for (i = 0; i < s.size; i++)
    if (strcmp(vertex, *(s.vertices + i)) == 0)
      return true;
  return false;
}

vertex_set uni(vertex_set a, vertex_set b)
{
  int i;
  vertex_set retValue, big, small;

  retValue.size = 0;
  retValue.vertices = NULL;

  if (a.size == 0 && b.size == 0)
    return retValue;
  else if (a.size >= b.size) {
    big = a;
    small = b;
  } else {
    big = b;
    small = a;
  }

  retValue.size = big.size;
  retValue.vertices = malloc(big.size * sizeof(char *));
  for (i = 0; i < big.size; i++)
    *(retValue.vertices + i) = strdup(*(big.vertices + i));

  for (i = 0; i < small.size; i++) {
    if (!in_set(*(small.vertices + i), big)) {
      retValue.size++;
      retValue.vertices = realloc(retValue.vertices, retValue.size * sizeof(char *));
      *(retValue.vertices + (retValue.size - 1)) = strdup(*(small.vertices + i));
    }
  }

  return retValue;
}

vertex_set inter(vertex_set a, vertex_set b)
{
  int i;
  vertex_set retValue;

  retValue.size = 0;
  retValue.vertices = NULL;

  if (a.size != 0 && b.size != 0) {
    for (i = 0; i < a.size; i++) {
      if (in_set(*(a.vertices + i), b)) {
        retValue.size++;
        retValue.vertices = realloc(retValue.vertices, retValue.size * sizeof(char *));
        *(retValue.vertices + (retValue.size - 1)) = strdup(*(a.vertices + i));
      }
    }
  }

  return retValue;
}

vertex_set minus(vertex_set a, vertex_set b)
{
  int i;
  vertex_set retValue;

  retValue.size = 0;
  retValue.vertices = NULL;

  if (a.size != 0) {
    if (b.size == 0) {
      retValue.size = a.size;
      retValue.vertices = malloc(a.size * sizeof(char *));
      for (i = 0; i < a.size; i++)
        *(retValue.vertices + i) = strdup(*(a.vertices + i));
    } else {
      for (i = 0; i < a.size; i++) {
        if (!in_set(*(a.vertices + i), b)) {
          retValue.size++;
          retValue.vertices = realloc(retValue.vertices, retValue.size * sizeof(char *));
          *(retValue.vertices + (retValue.size - 1)) = strdup(*(a.vertices + i));
        }
      }
    }
  }

  return retValue;
}

vertex_set find_pivot(lan *l, vertex_set p, vertex_set x)
{
  char *v;
  int   i, j, k, neighlen, max;
  node  pivot;
  vertex_set u, m;

  u = uni(p, x);
  max = INT32_MIN;
  for (i = 0; i < u.size; i++) {
    v = *(u.vertices + i);
    j = ctoi(*v);
    k = ctoi(*(v + 1));
    if ((neighlen =  l->net[j][k].neighlen) > max) {
      max = neighlen;
      pivot = l->net[j][k];
    }
  }
  free_vertex_set(u);

  m.size = pivot.neighlen;
  m.vertices = pivot.neighbours;

  return minus(p, m);
}

vertex_set alloc_single_vertex(char *v)
{
  vertex_set retValue;

  retValue.size = 1;
  retValue.vertices = malloc(sizeof(char *));
  *retValue.vertices = strdup(v);

  return retValue;
}

vertex_set neighbours(char *v, lan *l)
{
  int i, j;
  node n;
  vertex_set retValue;

  i = ctoi(*v);
  j = ctoi(*(v + 1));
  n = l->net[i][j];

  retValue.size = n.neighlen;
  retValue.vertices = malloc(retValue.size * sizeof(char *));
  for (i = 0; i < retValue.size; i++)
    *(retValue.vertices + i) = strdup(*(n.neighbours + i));

  return retValue;
}

void bron_kerbosch(vertex_set r, vertex_set p, vertex_set x, lan *l, cliques *res)
{
  char *v;
  int i;
  vertex_set pivot, single, neigh, tofree;

  if (p.size == 0 && x.size == 0) {
    res->vslen++;
    res->vs = realloc(res->vs, res->vslen * sizeof(vertex_set));
    *(res->vs + (res->vslen - 1))  = r;
  } else {
    pivot = find_pivot(l, p, x);
    for (i = 0; i < pivot.size; i++) {
      v = *(pivot.vertices + i);
      single = alloc_single_vertex(v);
      neigh = neighbours(v, l);

      bron_kerbosch(uni(r, single), inter(p, neigh), inter(x, neigh), l, res);

      tofree = p;
      p = minus(p, single);
      free_vertex_set(tofree);

      tofree = x;
      x = uni(x, single);
      free_vertex_set(tofree);

      free_vertex_set(single);
      free_vertex_set(neigh);
    }
  }
}
