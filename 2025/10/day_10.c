#include "day_10.h"

void part_1(void);

int main(void)
{
  part_1();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *toFree;
  diag d;
  btns btns;

  ifp = fopen("example_input.txt", "r");
  linecap = 0;
  line = NULL;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;
    d = get_diag(&line);
    print_diag(d);

    btns = alloc_btns(&line, d.len);
    print_btns(btns);
    free_btns(btns);

    free(toFree);
    line = NULL;
  }

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

jolt alloc_jolt(char **linep)
{
  char c, *line, *start;
  int len;
  jolt retValue;

  line = start = *linep;
  len = 1;
  while ((c = *line++) != '}')
    if (c == ',')
      len++;
  line = start;
  retValue.len = len;
  retValue.reqs = malloc(len * sizeof(int));

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

  retValue.reqs -= retValue.len;
  return retValue;
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
