#include "day_24.h"

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
  int nkeys, nvalues;
  bool *zvalues;
  input in;
  wirelist **tab;

  in = alloc_input();
  print_conns(in.conns);

  nkeys = nvalues = 0;
  tab = alloc_table(in.wires, &nkeys, &nvalues);
  run_simulation(in.conns, tab, nkeys);

  zvalues = get_values(tab, nkeys, nvalues, 'z');
  printf("ans: %ld\n", to_long(zvalues, nvalues));

  free_tree(in.wires);
  free_conns(in.conns);
  free_table(tab, nkeys);
  free(zvalues);
}

void part_2(void)
{
  char xwire[4], ywire[4], zwire[4], *xxory, *curcarry, *xandy, *xxoryandcarry, *nextcarry;
  int i, nkeys, nvalues;
  unsigned long xval, yval, zgoodval, zbadval;
  bool *xvalues, *yvalues, *zvalues;
  input in;
  wirelist **tab;

  in = alloc_input();

  nkeys = nvalues = 0;
  tab = alloc_table(in.wires, &nkeys, &nvalues);
  run_simulation(in.conns, tab, nkeys);

  xvalues = get_values(tab, nkeys, nvalues - 1, X);
  xval = to_long(xvalues, nvalues - 1);
  yvalues = get_values(tab, nkeys, nvalues - 1, Y);
  yval = to_long(yvalues, nvalues - 1);
  zvalues = get_values(tab, nkeys, nvalues, Z);
  zbadval = to_long(zvalues, nvalues);

  bit_print(xval);
  printf(" x\n");
  bit_print(yval);
  printf(" y\n");
  bit_print(zbadval);
  printf(" zbad\n");

  zgoodval = xval + yval;
  bit_print(zgoodval);
  printf(" zgood\n");

  bit_print(zgoodval ^ zbadval);
  printf(" diff\n");

  for (i = 1; i < nvalues - 1; i++) {
    snprintf(xwire, 4, "%c%02d", X, i);
    snprintf(ywire, 4, "%c%02d", Y, i);
    snprintf(zwire, 4, "%c%02d", Z, i);

    xxory = x_xor_y(in.conns, xwire, ywire);
    if (xxory != NULL)
      printf("%s XOR %s -> %s\n", xwire, ywire, xxory);
    else {
      printf("failed to find %s XOR %s -> _\n\n", xwire, ywire);
      continue;
    }

    curcarry = x_xor_y_xor_carry(in.conns, xxory, zwire);
    if (curcarry != NULL)
      printf("%s XOR %s -> %s\n", xxory, curcarry, zwire);
    else {
      printf("failed to find %s XOR _ -> %s\n\n", xxory, zwire);
      continue;
    }

    xandy = x_and_y(in.conns, xwire, ywire);
    if (xandy != NULL)
      printf("%s AND %s -> %s\n", xwire, ywire, xandy);
    else {
      printf("failed to find %s AND %s -> _\n\n", xwire, ywire);
      continue;
    }

    xxoryandcarry = x_xor_y_and_carry(in.conns, xxory, curcarry);
    if (xxoryandcarry != NULL)
      printf("%s AND %s -> %s\n", xxory, curcarry, xxoryandcarry);
    else {
      printf("failed to find %s AND %s -> _\n\n", xxory, curcarry);
      continue;
    }

    nextcarry = next_carry(in.conns, xxoryandcarry, xandy);
    if (nextcarry != NULL)
      printf("%s  OR %s -> %s\n", xxoryandcarry, xandy, nextcarry);
    else {
      printf("failed to find %s OR %s -> _\n\n", xxoryandcarry, xandy);
      continue;
    }
    printf("%s  +  %s -> %s\n\n", xwire, ywire, zwire);
  }

  free_tree(in.wires);
  free_conns(in.conns);
  free_table(tab, nkeys);
  free(xvalues);
  free(yvalues);
  free(zvalues);
}

input alloc_input(void)
{
  char *line, *tofree, *wire, *lwire, *gate, *rwire, *owire;
  input retValue;
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  tnode *wires;
  connlist *conns;
  regex_t regex;
  regmatch_t matches[5];

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  wires = NULL;
  conns = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    tofree = line;
    wire = strsep(&line, ": ");
    wires = add_wire_value(wires, wire, line);

    free(tofree);
    line = NULL;
  }

  free(line);
  line = NULL;

  if (regcomp(&regex, "([0-9a-z]+) (AND|OR|XOR) ([0-9a-z]+) -> ([0-9a-z]+)", REG_EXTENDED | REG_ICASE))
    printf("RegEx compilation error\n");
  else {
    while ((linelen = getline(&line, &linecap, ifp)) > 1) {
      regexec(&regex, line, 5, matches, 0);

      lwire = get_match(line, matches[1]);
      gate = get_match(line, matches[2]);
      rwire = get_match(line, matches[3]);
      owire = get_match(line, matches[4]);

      wires = add_wire(wires, lwire);
      wires = add_wire(wires, rwire);
      wires = add_wire(wires, owire);

      conns = add_conn(conns, lwire, gate, rwire, owire);

      free(lwire);
      free(gate);
      free(rwire);
      free(owire);
      free(line);
      line = NULL;
    }
  }

  free(line);
  regfree(&regex);
  fclose(ifp);

  retValue.wires = wires;
  retValue.conns = conns;

  return retValue;
}

tnode *add_wire(tnode *p, char *w)
{
  int cond;

  if (p == NULL) {
    p = malloc(sizeof(tnode));
    p->w.name = strdup(w);
    p->w.isset = false;
    p->w.value = false;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(w, p->w.name)) != 0) {
    if (cond < 0)
      p->left = add_wire(p->left, w);
    else
      p->right = add_wire(p->right, w);
  }

  return p;
}

tnode *add_wire_value(tnode *p, char *w, char *v)
{
  int cond;

  if (p == NULL) {
    p = malloc(sizeof(tnode));
    p->w.name = strdup(w);
    p->w.isset = true;
    p->w.value = *(v + 1) == '1' ? true : false;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(w, p->w.name)) != 0) {
    if (cond < 0)
      p->left = add_wire_value(p->left, w, v);
    else
      p->right = add_wire_value(p->right, w, v);
  }

  return p;
}

void node_count(tnode *p, int *nkeys, int *zwires)
{
  if (p != NULL) {
    *nkeys += 1;
    if (*(p->w.name) == 'z')
      *zwires += 1;
    node_count(p->left, nkeys, zwires);
    node_count(p->right, nkeys, zwires);
  }
}

void print_tree(tnode *p)
{
  if (p != NULL) {
    print_tree(p->left);
    printf("node: %s, isset: %s, value: %s\n", p->w.name, p->w.isset ? "true" : "false", p->w.value ? "true" : "false");
    print_tree(p->right);
  }
}

void free_tree(tnode *p)
{
  if (p->left != NULL)
    free_tree(p->left);

  if (p->right != NULL)
    free_tree(p->right);

  free(p->w.name);
  free(p);
}

connlist *add_conn(connlist *p, char *l, char *g, char *r, char *o)
{
  if (p == NULL) {
    p = malloc(sizeof(connlist));
    p->lwire = strdup(l);
    p->rwire = strdup(r);
    p->outwire = strdup(o);
    p->simulated = false;
    p->next = NULL;

    if (strcmp(g, "OR") == 0)
      p->g = OR;
    else if (strcmp(g, "XOR") == 0)
      p->g = XOR;
    else
      p->g = AND;
  }
  else
    p->next = add_conn(p->next, l, g, r, o);
  return p;
}

void print_conns(connlist *p)
{
  char gate[4];
  while (p != NULL) {
    if (p->g == OR)
      snprintf(gate, 4, "%3s", "OR");
    else if (p->g == XOR)
      snprintf(gate, 4, "%3s", "XOR");
    else
      snprintf(gate, 4, "%3s", "AND");
    printf("{%s %s %s -> %s}, simulated: %s\n", p->lwire, gate, p->rwire, p->outwire, p->simulated ? "true" : "false");
    p = p->next;
  }
}

void free_conns(connlist *p)
{
  connlist *tofree;

  while (p != NULL) {
    tofree = p;
    p = p->next;

    free(tofree->lwire);
    free(tofree->rwire);
    free(tofree->outwire);
    free(tofree);
  }
}

wirelist **alloc_table(tnode *p, int *nkeys, int *nvalues)
{
  int i;
  wirelist **retValue;

  node_count(p, nkeys, nvalues);

  retValue = malloc(*nkeys * sizeof(wirelist *));
  for (i = 0; i < *nkeys; i++)
    *(retValue + i) = NULL;

  add_to_table(p, retValue, *nkeys);

  return retValue;
}

void add_to_table(tnode *p, wirelist **tab, int nkeys)
{
  int i;
  wirelist *wlist;

  if (p != NULL) {
    i = hash(p->w.name, nkeys);
    wlist = malloc(sizeof(wirelist));
    wlist->w.name = strdup(p->w.name);
    wlist->w.isset = p->w.isset;
    wlist->w.value = p->w.value;
    wlist->next = *(tab + i);
    *(tab + i) = wlist;

    add_to_table(p->left, tab, nkeys);
    add_to_table(p->right, tab, nkeys);
  }
}

wire get_wire(wirelist **tab, int nkeys, char *w)
{
  unsigned long i;
  wirelist *p;

  i = hash(w, nkeys);
  p = *(tab + i);
  while (strcmp(p->w.name, w) != 0)
    p = p->next;

  return p->w;
}

void set_wire(wirelist **tab, int nkeys, char *w, bool value)
{
  unsigned long i;
  wirelist *p;

  i = hash(w, nkeys);
  p = *(tab + i);
  while (strcmp(p->w.name, w) != 0)
    p = p->next;

  p->w.isset = true;
  p->w.value = value;
}

void print_table(wirelist **tab, int nkeys)
{
  int i;
  wirelist *wlist;

  for (i = 0; i < nkeys; i++) {
    printf("index: %d has wirelist:\n", i);
    wlist = *(tab + i);
    while (wlist != NULL) {
      printf("name: %s, isset: %s, value: %d\n", wlist->w.name, wlist->w.isset ? "true" : "false", wlist->w.value);
      wlist = wlist->next;
    }
  }
}

void free_table(wirelist **tab, int nkeys)
{
  int i;
  wirelist *wlist, *tofree;

  for (i = 0; i < nkeys; i++) {
    wlist = *(tab + i);
    while (wlist != NULL) {
      tofree = wlist;
      wlist = wlist->next;
      free(tofree->w.name);
      free(tofree);
    }
  }
  free(tab);
}

unsigned long hash(char *s, int nkeys)
{
  unsigned long hashval;

  for (hashval = 5381; *s != '\0'; s++)
    hashval = *s + 33 * hashval;
  return hashval % nkeys;
}

char *get_match(char *line, regmatch_t match)
{
  char *retValue;
  regoff_t length;

  length = match.rm_eo - match.rm_so;
  retValue = malloc((length + 1) * sizeof(char));
  (void) strncpy(retValue, line + match.rm_so, length);
  *(retValue + length) = '\0';

  return retValue;
}

void run_simulation(connlist *conns, wirelist **tab, int nkeys)
{
  bool allrun, value;
  wire l, r;
  connlist *head;

  head = conns;
  allrun = false;
  while (!allrun) {
    allrun = true;

    while (conns != NULL) {
      l = get_wire(tab, nkeys, conns->lwire);
      r = get_wire(tab, nkeys, conns->rwire);

      if (l.isset && r.isset) {
        if (conns->g == OR)
          value = l.value | r.value;
        else if (conns->g == XOR )
          value = l.value ^ r.value;
        else
          value = l.value & r.value;
        set_wire(tab, nkeys, conns->outwire, value);
      }
      else
        allrun = false;

      conns = conns->next;
    }
    conns = head;
  }
}

bool *get_values(wirelist **tab, int nkeys, int nvalues, char c)
{
  char *n;
  int i, j;
  bool *retValue;
  wirelist *p;

  retValue = malloc(nvalues * sizeof(bool));
  for (i = 0; i < nvalues; i++)
    *(retValue + i) = false;

  for (i = 0; i < nkeys; i++) {
    p = *(tab + i);
    while (p != NULL) {
      n = p->w.name;
      if (*n == c) {
        j = atoi(n + 1);
        *(retValue + j) = p->w.value;
      }
      p = p->next;
    }
  }

  return retValue;
}

unsigned long to_long(bool *values, int nvalues)
{
  int i;
  unsigned long retValue, one;

  one = 1;
  retValue = 0;
  for (i = 0; i < nvalues; i++)
    if (*(values + i))
      retValue |= one << i;

  return retValue;
}

void bit_print(unsigned long a)
{
  int i, n;
  unsigned long one, mask;

  n = sizeof(long) * CHAR_BIT;
  one = 1;
  mask = one << (n - 1);

  for(i = 1; i <= n; ++i) {
    putchar((a & mask) ? '1' : '0');
    a <<= 1;
    if (i % 4 == 0 && i < n)
      putchar(' ');
  }
}

char *x_xor_y(connlist *c, char *x, char *y)
{
  while (c != NULL) {
    if (c->g == XOR &&
        ((strcmp(c->lwire, x) == 0 && strcmp(c->rwire, y) == 0) ||
         (strcmp(c->lwire, y) == 0 && strcmp(c->rwire, x) == 0)))
      return c->outwire;
    c = c->next;
  }
  return NULL;
}

char *x_xor_y_xor_carry(connlist *c, char *xxory, char *z)
{
  while (c != NULL) {
    if (c->g == XOR &&
        strcmp(c->outwire, z) == 0) {
      if (strcmp(c->lwire, xxory) == 0)
        return c->rwire;
      else if (strcmp(c->rwire, xxory) == 0)
        return c->lwire;
    }
    c = c->next;
  }
  return NULL;
}

char *x_and_y(connlist *c, char *x, char *y)
{
  while (c != NULL) {
    if (c->g == AND &&
        ((strcmp(c->lwire, x) == 0 && strcmp(c->rwire, y) == 0) ||
         (strcmp(c->lwire, y) == 0 && strcmp(c->rwire, x) == 0)))
      return c->outwire;
    c = c->next;
  }
  return NULL;
}

char *x_xor_y_and_carry(connlist *c, char *xxory, char *carry)
{
  while (c != NULL) {
    if (c->g == AND &&
        ((strcmp(c->lwire, xxory) == 0 && strcmp(c->rwire, carry) == 0) ||
         (strcmp(c->lwire, carry) == 0 && strcmp(c->rwire, xxory) == 0)))
      return c->outwire;
    c = c->next;
  }
  return NULL;
}

char *next_carry(connlist *c, char *xxoryandcarry, char *xandy)
{
  while (c != NULL) {
    if (c->g == OR &&
        ((strcmp(c->lwire, xxoryandcarry) == 0 && strcmp(c->rwire, xandy) == 0) ||
         (strcmp(c->lwire, xandy) == 0 && strcmp(c->rwire, xxoryandcarry) == 0)))
      return c->outwire;
    c = c->next;
  }
  return NULL;
}
