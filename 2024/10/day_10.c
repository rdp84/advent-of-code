#include "day_10.h"

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
  char **map;
  int rows, cols, sum;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  sum = sum_all_trailhead_scores(map, rows, cols);
  free_map(map, rows);
  printf("sum of all trailheads: %d\n", sum);
}

void part_2(void)
{
  char **map;
  int rows, cols, sum;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  sum = sum_all_trailhead_scores(map, rows, cols);
  free_map(map, rows);
  printf("sum of all trailheads: %d\n", sum);
}

char **alloc_map(int *rows, int *cols)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  linelen = getline(&line, &linecap, ifp);
  if (linelen < 0) {
    printf("error reading file");
    return NULL;
  }

  *rows = 1;
  *cols = linelen - 1;
  retValue = malloc(sizeof(char *));
  *retValue = line;
  line = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    retValue = realloc(retValue, (*rows + 1) * sizeof(char *));
    *(retValue + *rows) = line;
    (*rows)++;
    line = NULL;
  }

  free(line);
  fclose(ifp);

  return retValue;
}

void free_map(char **map, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    free(*(map + i));
  free(map);
}

void print_map(char **map, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
      printf("%s", *(map + i));
  printf("\n");
}

int sum_all_trailhead_scores(char **map, int rows, int cols)
{
  int i, j, retValue;
  trailhead *head, *tail;
  reachable_nine *toFreeNines;

  retValue = 0;
  head = NULL;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      if (*(*(map + i) + j) == '0') {

        if (head == NULL) {
          head = malloc(sizeof(trailhead));
          head->row = i;
          head->col = j;
          head->visits = 0;
          head->nines = NULL;
          head->next = NULL;

          walk_from_trailhead(map, i, j, rows, cols, head);
        }
        else {
          tail = head;
          while (tail->next != NULL)
            tail = tail->next;

          tail->next = malloc(sizeof(trailhead));
          tail->next->row = i;
          tail->next->col = j;
          tail->next->visits = 0;
          tail->next->nines = NULL;
          tail->next->next = NULL;

          walk_from_trailhead(map, i, j, rows, cols, tail->next);
        }
      }
    }
  }
  
  while (head != NULL) {
    retValue += head->visits;

    while (head->nines != NULL) {
      toFreeNines = head->nines;
      head->nines = head->nines->next;
      free(toFreeNines);
    }

    tail = head;
    head = head->next;
    free(tail);
  }

  return retValue;
}

void walk_from_trailhead(char **map, int i, int j, int rows, int cols, trailhead *th)
{
  char c;

  c = *(*(map + i) + j);
  
  if (c == '9') {
    /* part 1 - only save a nine we have not visited before */
    /* add_unique_reachable_nine(th, i, j); */

    /* part 2 - save all nines for this trailhead */
    add_reachable_nine(th, i, j);
  }
  else {
    if (i > 0 &&
        c + 1 == *(*(map + (i - 1)) + j)) {
      walk_from_trailhead(map, i - 1, j, rows, cols, th);
    }

    if (j < cols - 1 &&
        c + 1 == *(*(map + i) + (j + 1))) {
      walk_from_trailhead(map, i, j + 1, rows, cols, th);
    }

    if (i < rows - 1 &&
        c + 1 == *(*(map + (i + 1)) + j)) {
      walk_from_trailhead(map, i + 1, j, rows, cols, th);
    }

    if (j > 0 &&
        c + 1 == *(*(map + i) + (j - 1))) {
      walk_from_trailhead(map, i, j - 1, rows, cols, th);
    }
  }
}

void add_unique_reachable_nine(trailhead *th, int i, int j)
{
  reachable_nine *tmp;

  if (th->nines == NULL) {
    th->visits = 1;
    th->nines = malloc(sizeof(reachable_nine));
    th->nines->row = i;
    th->nines->col = j;
    th->nines->next = NULL;
  }
  else {
    tmp = th->nines;
    while (tmp->next != NULL && !(tmp->row == i && tmp->col == j))
      tmp = tmp->next;

    if (tmp->next == NULL && !(tmp->row == i && (tmp->col == j))) {
      th->visits += 1;
      tmp->next = malloc(sizeof(reachable_nine));
      tmp->next->row = i;
      tmp->next->col = j;
      tmp->next->next = NULL;
    }
  }
}

void add_reachable_nine(trailhead *th, int i, int j)
{
  reachable_nine *tmp;

  if (th->nines == NULL) {
    th->visits = 1;
    th->nines = malloc(sizeof(reachable_nine));
    th->nines->row = i;
    th->nines->col = j;
    th->nines->next = NULL;
  }
  else {
    tmp = th->nines;
    while (tmp->next != NULL)
      tmp = tmp->next;
    th->visits += 1;
    tmp->next = malloc(sizeof(reachable_nine));
    tmp->next->row = i;
    tmp->next->col = j;
    tmp->next->next = NULL;
  }
}
