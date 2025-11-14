#include "day_16.h"

void part_1(void);

int main(void)
{
  part_1();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  char **map;
  int i, j, foundStart, rows, cols, min;
  visited_locations *north, *east, *south;
  queue q;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  print_map(map, rows, cols);

  foundStart = 1;
  for (i = 0; i < rows && foundStart; i++) {
    for (j = 0; j < cols; j++) {
      if (*(*(map + i) + j) == START) {
        printf("Found start at (%d, %d)\n", i, j);
        foundStart = 0;
        break;
      }
    }
  }

  printf("Start at (%d, %d)\n", --i, j);
  initialize(&q);

  free_map(map, rows);
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
  *rows = *cols = 0;

  retValue = malloc(sizeof(char *));
  *cols = getline(&line, &linecap, ifp) - 1;
  *(retValue) = line;
  (*rows)++;
  line = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    retValue = realloc(retValue, (*rows + 1) * sizeof(char *));
    *(retValue + *rows) = line;
    (*rows)++;
    line = NULL;
  }

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

void print_map(char **input, int rows, int cols)
{
  int i, j;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++)
      printf("%c", *(*(input + i) + j));
    printf("\n");
  }
}

queue *alloc_queue()
{
  queue *retValue;

  retValue = malloc(sizeof(queue));
  retValue->cnt = 0;
  retValue->front = retValue->rear = NULL;
}

void enqueue(int s, int r, int c, int rDir, int cDir, queue *q)
{
  elem *e, *i, *j;

  e = malloc(sizeof(elem));
  e->score = s;
  e->row = r;
  e->col = c;
  e->rowDir = rDir;
  e->colDir = cDir;
  e->next = NULL;

  if (!empty(q)) {
    if (e->score < q->front->score) {
      e->next = q->front;
      q->front = e;
    } else if (q->front == q->rear) {
      q->front->next = e;
      q->rear = e;
    } else if (e->score >= q->rear->score) {
      q->rear->next = e;
      q->rear = e;
    } else {
      i = q->front;
      j = q->front->next;
      while (e->score > j->score) {
        i = j;
        j = j->next;
      }
      e->next = j;
      i->next = e;
    }
  } else 
    q->front = q->rear = e;

  q->cnt++;
}

bool empty(queue *q)
{
  return q->cnt == 0;
}

int pop_path(paths *paths)
{
  int score;
  path *toFree;

  score = paths->top->score;
  toFree = paths->top;
  paths->cnt--; 
  paths->top = paths->top->next;
  free(toFree);

  return score;
}

int peek_path(paths *paths)
{
  return paths->top->score;
}

location *alloc_location(int r, int c, int rDir, int cDir)
{
  location *retValue;

  retValue = malloc(sizeof(location));
  retValue->row = r;
  retValue->col = c;
  retValue->rowDir = rDir;
  retValue->cDir = cDir;
  retValue->next = NULL;

  return retValue;
}

void add_location(location *head, int r, int c, int rDir, int cDir)
{
  location *l;

  l = malloc(sizeof(location));
  l->row = r;
  l->col = c;
  l->rowDir = rDir;
  l->colDir = cDir;
  l->next = NULL;

  while (head->next != NULL)
    head = head->next;

  head->next = l;
}

bool is_looping(location *head, int r, int c, int rDir, int cDir)
{
  bool retValue;

  retValue = false;
  while (head != NULL && !retValue) {
    retValue = head->row == r && head->col == c && head->rowDir == rDir && head->colDir == cDir;
    head = head->next;
  }

  return retValue;
}

void free_location(location *l)
{
  location *toFree;

  while (l != NULL) {
    toFree = l;
    l = l->next;
    free(toFree);
  }
}
