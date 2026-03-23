#include "day_10.h"
#include "queue.h"
#include "solver.h"

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
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *toFree;
  bool configured;
  int i, ans;
  diag d;
  btns b;
  elem e, newelem;

  ifp = fopen("example_input.txt", "r");
  linecap = 0;
  line = NULL;
  ans = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;
    d = get_diag(&line);
    b = alloc_btns(&line, d.len);
    print_btns(b);
    alloc_queue(d.len);
    e.presses = e.val = 0;
    enqueue(e);

    configured = false;
    while (!configured) {
      e = dequeue();
      for (i = 0; i < b.len; i++) {
        newelem.presses = e.presses + 1;
        if ((newelem.val = e.val ^ *(b.btns + i)) == d.ilights) {
          ans += newelem.presses;
          configured = true;
          break;
        }
        else
          enqueue(newelem);
      }
    }
    free_queue();
    free_btns(b);
    free(toFree);
    line = NULL;
  }

  printf("ans: %d\n", ans);
  free(line);
  fclose(ifp);
}

void part_2(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *toFree;
  unsigned optidx;
  int64_t ans;
  diag d;
  btns b;
  jolt j;
  Z3_context ctx;
  Z3_optimize opt;
  Z3_ast *vars;

  ifp = fopen("example_input.txt", "r");
  linecap = 0;
  line = NULL;
  ans = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;
    d = get_diag(&line);
    b = alloc_btns(&line, d.len);
    j = alloc_jolt(&line, d.len);

    ctx = mk_context();
    opt = mk_optimize(ctx);
    vars = mk_btn_vars(ctx, opt, b);
    optidx = mk_sum(ctx, opt, vars, b.len);
    mk_eqns(ctx, opt, vars, b, j);
    ans += get_sum(ctx, opt, Z3_L_TRUE, optidx);

    free_z3(ctx, opt, vars);
    free_btns(b);
    free_jolt(j);
    free(toFree);
    line = NULL;
  }

  printf("ans: %lu\n", ans);
  free(line);
  fclose(ifp);
}

diag get_diag(char **linep)
{
  char c, *line, *start;
  int len;
  diag retValue;

  line = *linep;
  start = ++line;
  while (*line++ != ']') ;
  *linep = line;
  retValue.len = len = (int) ((line - start) - 1);

  retValue.ilights = 0;
  while ((c = *start++) != ']') {
    if (c == '#')
      retValue.ilights |= 1 << (len - 1);
    len--;
  }

  return retValue;
}

void print_diag(diag d)
{
  printf("diag has %d indicator lights:\n", d.len);
  bit_print(d.ilights);
}

btns alloc_btns(char **linep, int nlights)
{
  char c, *line;
  int i;
  btns retValue;

  line = *linep;
  retValue.len = 0;
  while ((c = *line++) != '{')
    if (c == '(')
      retValue.len++;
  retValue.btns = malloc(retValue.len * sizeof(int));
  for (i = 0; i < retValue.len; i++)
    *(retValue.btns + i) = 0;

  line = *linep;
  i = 0;
  while (*line != '{') {
    while (*line++ != '(') ;

    while ((c = *line++) != ')')
      if (isdigit(c))
        *(retValue.btns + i) |= 1 << (nlights - (c - '0' + 1));
    i++;

    while (isspace(*++line)) ;
  }

  *linep = line++;
  return retValue;
}

void print_btns(btns b)
{
  int i;

  printf("Number of buttons: %d\n", b.len);
  for (i = 0; i < b.len; i++)
    bit_print(*(b.btns + i));
}

void free_btns(btns b)
{
  free(b.btns);
}

jolt alloc_jolt(char **linep, int nlights)
{
  char c, *line, *start;
  jolt retValue;

  retValue.len = nlights;
  retValue.reqs = malloc(retValue.len * sizeof(int));

  line = *linep;
  while (!isdigit(*line))
    line++;

  start = line;
  while ((c = *line++) != '}') {
    if (c == ',') {
      *(line - 1) = '\0';
      *(retValue.reqs++) = atoi(start);
      start = line;
    }
  }

  if (c == '}') {
    *(line - 1) = '\0';
    *(retValue.reqs) = atoi(start);
  }

  retValue.reqs -= (retValue.len - 1);
  return retValue;
}

void print_jolt(jolt j)
{
  int i;

  printf("Number of jolts: %d\n", j.len);
  for (i = 0; i < j.len; i++)
    printf("%d\n", *(j.reqs + i));
}

void free_jolt(jolt j)
{
  free(j.reqs);
}

void bit_print(int a)
{
  int i, n;
  long b, mask;

  b = (long) a;
  n = sizeof(int) * CHAR_BIT;
  mask = (long) 1 << (n - 1);
  for(i = 1; i <= n; ++i) {
    putchar((b & mask) ? '1' : '0');
    b <<= 1;
    if (i % CHAR_BIT == 0 && i < n)
      putchar(' ');
  }
  putchar('\n');
}
