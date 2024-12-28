#include "day_5.h"

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
  ordering_rules *or;

  ifp = fopen("input.txt", "r");
  or = alloc_ordering_rules(ifp);
  parse_updates(or, ifp);
  free_ordering_rules(or);

  fclose(ifp);
}

void part_2(void)
{
  FILE *ifp;
  ordering_rules *or;

  ifp = fopen("input.txt", "r");
  or = alloc_ordering_rules(ifp);
  modify_updates(or, ifp);
  free_ordering_rules(or);

  fclose(ifp);
}

ordering_rules *alloc_ordering_rules(FILE *ifp)
{
  size_t linecap;
  ssize_t linelen;
  char *line;
  int leading, trailing;
  regex_t regex;
  regmatch_t matches[3];
  ordering_rules *retValue, *tmpOR;
  rule *r, *tmpR;

  retValue = NULL;
  line = NULL;
  linecap = 0;

  if (regcomp(&regex, "^([0-9]+)\\|([0-9]+)\n$", REG_EXTENDED) != 0) {
    printf("RegEx compilation error");
    return retValue;
  }

  while ((linelen = getline(&line, &linecap, ifp)) > 0 &&
         *line != '\n') {
    regexec(&regex, line, 3, matches, 0);
    leading = get_page(line, matches[1]);
    trailing = get_page(line, matches[2]);

    r = malloc(sizeof(rule));
    r->page = trailing;
    r->next = NULL;

    if (retValue == NULL) {
      retValue = malloc(sizeof(ordering_rules));
      retValue->page = leading;
      retValue->rules = r;
      retValue->next = NULL;
    }
    else {
      tmpOR = retValue;
      while (tmpOR->page != leading && tmpOR->next != NULL)
        tmpOR = tmpOR->next;

      if (tmpOR->page == leading) {
        tmpR = tmpOR->rules;
        while (tmpR->next != NULL)
          tmpR = tmpR->next;
        tmpR->next = r;
      }
      else {
        tmpOR->next = malloc(sizeof(ordering_rules));
        tmpOR->next->page = leading;
        tmpOR->next->rules = r;
        tmpOR->next->next = NULL;
      }
    }

    free(line);
    line = NULL;
    linecap = 0;
  }
  free(line);
  return retValue;
}

void parse_updates(ordering_rules *or, FILE *ifp)
{
  size_t linecap;
  ssize_t linelen;
  char *line, *token, *toFree;
  int *update, total, correct, middleCount, n, i, j;
  rule *r, *headR;
  bool isInOrder;

  total = correct = middleCount = 0;
  update = malloc(sizeof(int));
  line = NULL;
  linecap = 0;

  while ((linelen = getline(&line, &linecap, ifp)) > 0 &&
         *line != '\n') {
    toFree = line;
    total++;
    n = 0;

    while ((token = strsep(&line, ",")) != NULL) {
      *(update + n) = atoi(token);
      n++;
      update = realloc(update, (n + 1) * sizeof(int));
    }

    for (i = 0, isInOrder = true; i < n - 1 && isInOrder; i++) {
      r = headR = get_rules(or, *(update + i));
      
      for (j = i + 1; j < n && isInOrder; j++) {
        while (r != NULL && r->page != *(update + j))
          r = r->next;

        if (r == NULL)
          isInOrder = false;
        else
          r = headR;
      }
    }

    if (isInOrder) {
      correct++;
      middleCount += *(update + (n / 2));
    }

    free(toFree);
    line = NULL;
    free(update);
    update = malloc(sizeof(int));
  }
  printf("total updates: %d, number of which were correct: %d, middleCount: %d\n", total, correct, middleCount);
  free(line);
  free(update);
}


void modify_updates(ordering_rules *or, FILE *ifp)
{
  size_t linecap;
  ssize_t linelen;
  char *line, *token, *toFree;
  int *update, total, modified, middleCount, n, i, j, tmp;
  rule *r, *headR;
  bool isModified;

  total = modified = middleCount = 0;
  update = malloc(sizeof(int));
  line = NULL;
  linecap = 0;

  while ((linelen = getline(&line, &linecap, ifp)) > 0 &&
         *line != '\n') {
    toFree = line;
    total++;
    n = 0;

    while ((token = strsep(&line, ",")) != NULL) {
      *(update + n) = atoi(token);
      n++;
      update = realloc(update, (n + 1) * sizeof(int));
    }

    isModified = false;
    for (i = 0; i < n - 1; i++) {
      r = headR = get_rules(or, *(update + i));
      
      for (j = i + 1; j < n; j++) {
        while (r != NULL && r->page != *(update + j))
          r = r->next;

        if (r == NULL) {
          isModified = true;
          tmp = *(update + i);
          *(update + i) = *(update + j);
          *(update + j) = tmp;
          r = headR = get_rules(or, *(update + i));
        }
        else
          r = headR;
      }
    }

    if (isModified) {
      modified++;
      middleCount += *(update + (n / 2));
    }

    free(toFree);
    line = NULL;
    free(update);
    update = malloc(sizeof(int));
  }
  printf("total updates: %d, number of which were modified: %d, middleCount: %d\n", total, modified, middleCount);
  free(line);
  free(update);
}

int get_page(char *line, regmatch_t match)
{
  char *page;
  regoff_t length;
  int retValue;

  length = match.rm_eo - match.rm_so;
  page = malloc((length + 1) * sizeof(char));
  strncpy(page, line + match.rm_so, length);
  *(page + length) = '\0';

  retValue = atoi(page);
  free(page);
  return retValue;
}

void print_rules(ordering_rules *or)
{
  rule *tmp;
  while (or != NULL) {
    printf("page %d:", or->page);
    tmp = or->rules;
    while (tmp != NULL) {
      printf(" %d", tmp->page);
      tmp = tmp->next;
    }
    printf("\n");
    or = or->next;
  }
}

rule *get_rules(ordering_rules *or, int page)
{
  rule *retValue;

  while (or != NULL && or->page != page)
    or = or->next;

  retValue = or == NULL ? NULL : or->rules;
  return retValue;
}

void free_ordering_rules(ordering_rules *or)
{
  ordering_rules *tmpOR;
  rule *r, *tmpR;

  while (or != NULL) {
    r = or->rules;
    while (r != NULL) {
      tmpR = r;
      r = r->next;
      free(tmpR);
    }
    tmpOR = or;
    or = or->next;
    free(tmpOR);
  }
}
