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
  int i, j, rows, cols;
  bool foundStart;
  queue *q;
  location *visited;
  elem e;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  print_map(map, rows, cols);

  foundStart = false;
  for (i = 0; i < rows && !foundStart; i++) {
    for (j = 0; j < cols; j++) {
      if (*(*(map + i) + j) == START) {
        printf("Found start at (%d, %d)\n", i, j);
        foundStart = true;
        break;
      }
    }
  }

  printf("Start at (%d, %d)\n", --i, j);

  q = alloc_queue();
  enqueue(0, i, j, 0, 1, q);
  print_queue(q);
  visited = alloc_location(i, j, 0, 1);

  while (!empty(q)) {
    e = dequeue(q);

    if (*(*(map + e.row) + e.col) != END) {
      move(e, map, q, visited);
      rotate(1, -1, e, map, q, visited); // clockwise
      rotate(-1, 1, e, map, q, visited); // counter-clockwise
    } else
      break;
  }

  /* print_queue(q); */
  printf("Found the end with a score of %d\n", e.score);
  free_map(map, rows);
  free_queue(q);
  free_location(visited);
}

void move(elem e, char **map, queue *q, location *l)
{
  int newRow, newCol;

  newRow = e.row + e.rowDir;
  newCol = e.col + e.colDir;

  if (*(*(map + newRow) + newCol) != WALL &&
      !is_looping(l, newRow, newCol, e.rowDir, e.colDir)) {
    enqueue(e.score + 1,  newRow, newCol, e.rowDir, e.colDir, q);
    add_location(l, newRow, newCol, e.rowDir, e.colDir);
  }
}

void rotate(int rowSign, int colSign, elem e, char **map, queue *q, location *l)
{
  int newRowDir, newColDir;

  newRowDir = rowSign * e.colDir;
  newColDir = colSign * e.rowDir;

  if (!is_looping(l, e.row, e.col, newRowDir, newColDir)) {
    enqueue(e.score + 1000, e.row, e.col, newRowDir, newColDir, q);
    add_location(l, e.row, e.col, newRowDir, newColDir);
  }
}

char **alloc_map(int *rows, int *cols)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("example_input.txt", "r");
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

queue *alloc_queue(void)
{
  queue *retValue;

  retValue = malloc(sizeof(queue));
  retValue->cnt = 0;
  retValue->front = retValue->rear = NULL;

  return retValue;
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

elem dequeue(queue *q)
{
  elem retValue, *toFree;

  toFree = q->front;
  q->front = q->front->next;
  q->cnt--;

  retValue.score = toFree->score;
  retValue.row = toFree->row;
  retValue.col = toFree->col;
  retValue.rowDir  = toFree->rowDir;
  retValue.colDir = toFree->colDir;
  retValue.next = NULL;

  free(toFree);
  return retValue;
}

bool empty(const queue *q)
{
  return q->cnt == 0;
}

void free_queue(queue *q)
{
  elem *toFree;

  while (q->front != NULL) {
    toFree = q->front;
    q->front = q->front->next;
    free(toFree);
  }

  free(q);
}

void print_queue(const queue *q)
{
  elem *e;

  printf("queue has %d elem\n", q->cnt);

  e = q->front;
  while (e != NULL) {
    printf("elem at (%d, %d), with score %d, moving in direction (%d, %d)\n",
           e->row, e->col, e->score, e->rowDir, e->colDir);
    e = e->next;
  }
}

location *alloc_location(int r, int c, int rDir, int cDir)
{
  location *retValue;

  retValue = malloc(sizeof(location));
  retValue->row = r;
  retValue->col = c;
  retValue->rowDir = rDir;
  retValue->colDir = cDir;
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
