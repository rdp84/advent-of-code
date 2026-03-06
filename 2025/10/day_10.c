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
  btnlist *btns;

  ifp = fopen("example_input.txt", "r");
  linecap = 0;
  line = NULL;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;
    d = alloc_diag(&line);
    print_diag(d);
    free_diag(d);

    btns = alloc_btns(&line);
    print_btns(btns);
    free_btns(btns);

    free(toFree);
    line = NULL;
  }

  free(line);
  fclose(ifp);
}

diag alloc_diag(char **linep)
{
  char *line, *start;
  diag retValue;

  line = *linep;
  start = ++line;
  while (*line++ != ']');
  *linep = line;
  retValue.len = (int) ((line - start) - 1);
  retValue.ilights = malloc(retValue.len * sizeof(int));

  while (*start != ']')
    *(retValue.ilights++) = *start++ == '.' ? 0 : 1;

  retValue.ilights -= retValue.len;
  return retValue;
}

void print_diag(diag d)
{
  int i;

  printf("diag has the indicator lights:\n");
  for (i = 0; i < d.len; i++)
    printf("%d", *(d.ilights + i));
  printf("\n");
}

void free_diag(diag d)
{
  free(d.ilights);
}

btnlist *alloc_btns(char **linep)
{
  char c, *line, *start;
  int len;
  btnlist *retValue, *head;

  retValue = NULL;
  line = *linep;
  while (*line != '{') {
    while (*line++ != '(') ;

    start = line;
    len = 1;
    while ((c = *line++) != ')')
      if (c == ',')
        len++;

    head = malloc(sizeof(btnlist));
    head->b.len = len;
    head->b.wiring = malloc(len * sizeof(int));
    while ((c = *start++) != ')')
      if (c != ',')
        *(head->b.wiring++) = c - '0';

    head->b.wiring -= len;
    head->next = retValue;
    retValue = head;

    while (isspace(*++line)) ;
  }

  *linep = line++;
  return retValue;
}

void print_btns(btnlist *head)
{
  int i;

  while (head != NULL) {
    printf("button has wiring schematic:\n");
    for (i = 0; i < head->b.len; i++)
      printf("%d ", *(head->b.wiring + i));
    printf("\n");
    head = head->next;
  }
}

void free_btns(btnlist *head)
{
  btnlist *toFree;
  
  while (head != NULL) {
    toFree = head;
    head = head->next;
    free(toFree->b.wiring);
    free(toFree);
  }
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
