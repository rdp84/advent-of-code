#include "day_19.h"

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
  towel *towels;
  int cnt, towelslen, designslen, designlen, i, j;
  char **designs, *design;
  long *cache;

  towelslen = 0;
  towels = alloc_towels(&towelslen);
  print_towels(towels, towelslen);

  designslen = 0;
  designs = alloc_designs(&designslen);
  print_designs(designs, designslen);

  cnt = 0;
  for (i = 0; i < designslen; i++) {
    design = *(designs + i);
    designlen = strlen(design);

    cache = malloc(designlen * sizeof(cache));
    for (j = 0; j < designlen; j++)
      *(cache + j) = -1;

    if (make_design(design, towels, towelslen, cache) > 0)
      cnt++;

    free(cache);
    cache = NULL;
  }

  printf("possible designs: %d\n", cnt);
  free(towels);
  free_designs(designs, designslen);
}

void part_2(void)
{
  towel *towels;
  int designslen, towelslen, designlen, i, j;
  long cnt, *cache;
  char **designs, *design;

  towelslen = 0;
  towels = alloc_towels(&towelslen);
  print_towels(towels, towelslen);

  designslen = 0;
  designs = alloc_designs(&designslen);
  print_designs(designs, designslen);

  cnt = 0;
  for (i = 0; i < designslen; i++) {
    design = *(designs + i);
    designlen = strlen(design);

    cache = malloc(designlen * sizeof(long));
    for (j = 0; j < designlen; j++)
      *(cache + j) = -1;

    cnt += make_design(design, towels, towelslen, cache);
    free(cache);
    cache = NULL;
  }

  printf("sum of all designs: %ld\n", cnt);
  free(towels);
  free_designs(designs, designslen);
}

long make_design(char *design, towel *towels, int towelslen, long *cache)
{
  int   deslen, i;
  long  cnt;
  towel t;

  cnt = 0;
  if (*design == '\0')
    cnt = 1;
  else {
    deslen = strlen(design);

    if (*(cache + deslen - 1) == -1) {
      for (i = 0; i < towelslen; i++) {
        t = *(towels + i);
        if (strnstr(design, t.pat, t.patlen) != NULL) {
          cnt += make_design(design + t.patlen, towels, towelslen, cache);
          *(cache + deslen - 1) = cnt;
        }
      }
    }
    else
      return *(cache + deslen - 1);
  }

  return cnt;
}

towel *alloc_towels(int *towelslen)
{
  FILE *ifp;
  size_t linecap;
  int len;
  char *line, *toFree;
  towel *retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  getline(&line, &linecap, ifp);

  toFree = line;
  retValue = NULL;
  *towelslen = 0;
  while (*line != '\0') {
    len = 0;
    while (isalpha(*line++))
      len++;

    retValue = realloc(retValue, (*towelslen + 1) * sizeof(towel));
    (*(retValue + *towelslen)).patlen = len;
    (*(retValue + *towelslen)).pat = alloc_pat(line, len);
    (*towelslen)++;

    if (*line == ' ')
      line++;
  }

  free(toFree);
  fclose(ifp);
  return retValue;
}

char *alloc_pat(char *line, int patlen)
{
  char *retValue;

  retValue = malloc((patlen + 1) * sizeof(char));
  *(retValue + patlen) = '\0';
  strncpy(retValue, line - 1 - patlen, patlen);

  return retValue;
}

char **alloc_designs(int *rows)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  retValue = NULL;
  *rows = 0;

  getline(&line, &linecap, ifp);
  free(line);
  line = NULL;
  getline(&line, &linecap, ifp);
  free(line);
  line = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    retValue = realloc(retValue, (*rows + 1) * sizeof(char *));
    *(line + linelen - 1) = '\0';
    *(retValue + *rows) = line;
    (*rows)++;
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

void print_towels(towel *towels, int towelslen)
{
  int i;

  for (i = 0; i < towelslen; i++)
    printf("towel has pattern %s with len %d\n", (*(towels + i)).pat, (*(towels + i)).patlen);
}

void print_designs(char **designs, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    printf("design: %s\n", *(designs + i));
}

void free_designs(char **designs, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    free(*(designs + i));

  free(designs);
}
