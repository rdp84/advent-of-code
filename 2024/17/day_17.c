#include "day_17.h"

void part_1(void);

int main(void)
{
  part_1();

  return EXIT_SUCCESS;
}

void part_1(void)
{
  char *prg;
  int a, b, c;

  a = b = c = 0;
  prg = alloc_program(&a, &b, &c);
  printf("a: %d\nb: %d\nc: %d\nprg: %s\n", a, b, c, prg);
  free(prg);
}

char *alloc_program(int *a, int *b, int *c)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *retValue, *line, *toFree, *pos, *num, chr, reg;
  int len, i;

  line = NULL;
  linecap = 0;
  ifp = fopen("example_input.txt", "r");

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    toFree = line;
    while ((chr = *(line++)) != A && chr != B && chr != C)
      ;

    reg = chr;
    line++;
    pos = line;
    while (*line++ != '\n')
      ;

    len = line - pos;
    num = malloc(len * sizeof(char));
    strlcpy(num, pos, len);

    if (reg == A)
      *a = atoi(num);
    else if (reg == B)
      *b = atoi(num);
    else
      *c = atoi(num);

    free(num);
    free(toFree);
    line = NULL;
  }

  free(line);
  line = NULL;
  getline(&line, &linecap, ifp);
  toFree = line;
  len = 0;
  while ((chr = *line++) != '\n')
    if (chr >= '0' && chr <= '9')
      len++;

  len++;
  retValue = malloc(len * sizeof(char));
  *(retValue + len - 1) = '\0';
  line = toFree;
  i = 0;
  while ((chr = *line++) != '\n')
    if (chr >= '0' && chr <= '9')
      *(retValue + i++) = chr;
  
  free(toFree);
  fclose(ifp);
  return retValue;
}
