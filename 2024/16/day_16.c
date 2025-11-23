#include "day_16.h"

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
  int i, j, rows, cols;
  bool foundStart;
  queue *q;
  location ***seen;
  path p;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  print_map(map, rows, cols);

  foundStart = false;
  for (i = 0; i < rows && !foundStart; i++) {
    for (j = 0; j < cols; j++) {
      if (*(*(map + i) + j) == START) {
        foundStart = true;
        break;
      }
    }
  }
  printf("Start at (%d, %d)\n", --i, j);

  seen = alloc_seen(rows, cols);

  q = alloc_queue();
  p.score = p.takenPos = 0;
  p.row = i;
  p.col = j;
  p.dir = EAST;
  p.taken = NULL;

  enqueue(p, q);
  while (!empty(q)) {
    p = dequeue(q);
    if (*(*(map + p.row) + p.col) == END) {
      printf("Found end with score %d and path taken:\n%s\n", p.score, p.taken);
      add_taken_to_map(p.taken, i, j, map);
      print_map(map, rows, cols);
      free(p.taken);
      break;
    }
    else if (p.score < seen_score(seen, p)) {
      rotate_clockwise(p, q);
      rotate_counter_clockwise(p, q);
      move(p, map, q);
    }
    else
      free(p.taken);
  }

  free_map(map, rows);
  free_seen(seen, rows, cols);
  free_queue(q);
}

void part_2(void)
{
  char c, **map, **bestTaken;
  int i, j, k, rows, cols, bestCnt, bestScore, seats;
  bool foundStart;
  queue *q;
  location ***seen;
  path p;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  print_map(map, rows, cols);

  foundStart = false;
  for (i = 0; i < rows && !foundStart; i++) {
    for (j = 0; j < cols; j++) {
      if (*(*(map + i) + j) == START) {
        foundStart = true;
        break;
      }
    }
  }
  printf("Start at (%d, %d)\n", --i, j);

  seen = alloc_seen(rows, cols);

  q = alloc_queue();
  p.score = p.takenPos = 0;
  p.row = i;
  p.col = j;
  p.dir = EAST;
  p.taken = NULL;

  bestTaken = NULL;
  bestCnt = 0;
  bestScore = INT32_MAX;

  enqueue(p, q);
  while (!empty(q)) {
    p = dequeue(q);

    if (p.score > bestScore) {
      printf("Going to break with score %d\n", p.score);
      break;
    } else if (*(*(map + p.row) + p.col) == END) {
      printf("Found a best path with a score of %d\n", p.score);
      bestScore = p.score;
      bestTaken = realloc(bestTaken, (bestCnt + 1) * sizeof(char *));
      *(bestTaken + bestCnt) = p.taken;
      bestCnt++;
    } else if (p.score <= seen_score(seen, p)) {
      rotate_clockwise(p, q);
      rotate_counter_clockwise(p, q);
      move(p, map, q);
    }
    else
      free(p.taken);
  }

  printf("Total number of paths with best score: %d\n", bestCnt);

  for (k = 0; k < bestCnt; k++) {
    add_taken_to_map(*(bestTaken + k), i, j, map);
    free(*(bestTaken + k));
  }

  print_map(map, rows, cols);

  seats = 0;
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      if ((c = *(*(map + i) + j)) == NORTH || c == EAST || c == SOUTH || c == WEST || c == END)
        seats++;
  printf("Number of seats: %d\n", seats);

  free(bestTaken);
  free_map(map, rows);
  free_seen(seen, rows, cols);
  free_queue(q);
}

/* Map functions */

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

/* Seen functions */

location ***alloc_seen(int rows, int cols)
{
  int i, j;
  location ***retValue;

  retValue = calloc(rows, sizeof(location *));
  for (i = 0; i < rows; i++) {
    *(retValue + i) = calloc(cols, sizeof(location *));
    for (j = 0; j < cols; j++)
      *(*(retValue + i) + j) = NULL;
  }

  return retValue;
}

int seen_score(location ***seen, path p)
{
  location *current, *next, *toAdd;
  int retScore;

  current = *(*(seen + p.row) + p.col);

  if (current == NULL) {
    current = malloc(sizeof(location));
    current->score = p.score;
    current->row = p.row;
    current->col = p.col;
    current->dir = p.dir;
    current->next = NULL;
    *(*(seen + p.row) + p.col) = current;

    return INT32_MAX;
  }
  else {
    next = current;
    while (next != NULL) {
      if (next->dir == p.dir) {
        retScore = next->score;
        if (p.score < next->score)
          next->score = p.score;

        return retScore;
      }
      current = next;
      next = next->next;
    }
    toAdd = malloc(sizeof(location));
    toAdd->score = p.score;
    toAdd->row = p.row;
    toAdd->col = p.col;
    toAdd->dir = p.dir;
    toAdd->score = p.score;
    toAdd->next = NULL;
    current->next = toAdd;

    return INT32_MAX;
  }
}

void free_seen(location ***seen, int rows, int cols)
{
  int i, j;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++)
      free_location(*(*(seen + i) + j));
    free(*(seen + i));
  }
  free(seen);
}

void free_location(location *head)
{
  location *toFree;

  while (head != NULL) {
    toFree = head;
    head = head->next;
    free(toFree);
  }
}

void print_location(location *l)
{
  while (l != NULL) {
    printf("location at (%d, %d), going in direction %c, with a score of %d\n",
           l->row, l->col, l->dir, l->score);
    l = l->next;
  }
}

/* Queue functions */

queue *alloc_queue(void)
{
  queue *retValue;

  retValue = malloc(sizeof(queue));
  retValue->cnt = 0;
  retValue->front = retValue->rear = NULL;

  return retValue;
}

void enqueue(path p, queue *q)
{
  elem *e, *i, *j;

  e = malloc(sizeof(elem));
  e->p = p;
  e->next = NULL;

  if (!empty(q)) {
    if (e->p.score < q->front->p.score) {
      e->next = q->front;
      q->front = e;
    } else if (q->front == q->rear) {
      q->front->next = e;
      q->rear = e;
    } else if (e->p.score >= q->rear->p.score) {
      q->rear->next = e;
      q->rear = e;
    } else {
      i = q->front;
      j = q->front->next;
      while (e->p.score > j->p.score) {
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

path dequeue(queue *q)
{
  elem *toFree;
  path retValue;

  toFree = q->front;
  q->front = q->front->next;
  q->cnt--;

  retValue.score = toFree->p.score;;
  retValue.row = toFree->p.row;
  retValue.col = toFree->p.col;
  retValue.dir = toFree->p.dir;
  retValue.takenPos = toFree->p.takenPos;
  retValue.taken = toFree->p.taken;

  free(toFree);
  return retValue;
}

bool empty(queue *q)
{
  return q->cnt == 0;
}

void free_queue(queue *q)
{
  elem *toFree;

  while (q->front != NULL) {
    toFree = q->front;
    q->front = q->front->next;
    free(toFree->p.taken);
    free(toFree);
  }
  free(q);
}

void print_queue(queue *q)
{
  elem *e;
  char c;
  int i;

  printf("queue has %d elem\n", q->cnt);

  e = q->front;
  while (e != NULL) {
    i = 0;
    if (e->p.taken != NULL) {
      while ((c = *(e->p.taken + i)) == NORTH || c == EAST || c == SOUTH || c == WEST)
        i++;
      *(e->p.taken + i) = '\0';
      printf("elem at (%d, %d), with score %d, moving in direction %c, has completed "\
             "the following %d steps: %s\n",
             e->p.row, e->p.col, e->p.score, e->p.dir, e->p.takenPos, e->p.taken);
      *(e->p.taken + i) = c;
    }
    else
      printf("elem at (%d, %d), with score %d, moving in direction %c, has completed "\
             "the following %d steps: NULL\n",
             e->p.row, e->p.col, e->p.score, e->p.dir, e->p.takenPos);

    e = e->next;
  }
}

/* Movement functions */

void rotate(path p, queue *q)
{
  char *currentTaken;

  currentTaken = p.taken;
  p.score += 1000;

  if (currentTaken != NULL) {
    p.taken = malloc_taken(p);
    p.taken = strcpy(p.taken, currentTaken);
  }
  else
    p.taken = NULL;

  enqueue(p, q);
}

void rotate_clockwise(path p, queue *q)
{
  if (p.dir == NORTH)
    p.dir = EAST;
  else if (p.dir == EAST)
    p.dir = SOUTH;
  else if (p.dir == SOUTH)
    p.dir = WEST;
  else
    p.dir = NORTH;

  rotate(p, q);
}

void rotate_counter_clockwise(path p, queue *q)
{
  if (p.dir == NORTH)
    p.dir = WEST;
  else if (p.dir == EAST)
    p.dir = NORTH;
  else if (p.dir == SOUTH)
    p.dir = EAST;
  else
    p.dir = SOUTH;

  rotate(p, q);
}

void move(path p, char **map, queue *q)
{
  int newRow, newCol;

  newRow = p.row;
  newCol = p.col;

  if (p.dir == NORTH)
    newRow--;
  else if (p.dir == EAST)
    newCol++;
  else if (p.dir == SOUTH)
    newRow++;
  else
    newCol--;

  if (*(*(map + newRow) + newCol) != WALL) {
    p.score++;
    p.row = newRow;
    p.col = newCol;

    if (p.takenPos % TAKEN_SIZE == 0) {
      p.taken = realloc_taken(p);
      taken_end(p) = '\0';
    }
    *(p.taken + p.takenPos) = p.dir;
    p.takenPos++;

    enqueue(p, q);
  }
  else
    free(p.taken);
}

void add_taken_to_map(char *taken, int startRow, int startCol, char **map)
{
  char c;

  while ((c = *taken++) == NORTH || c == EAST || c == SOUTH || c == WEST) {
    *(*(map + startRow) + startCol) = c;

    if (c == NORTH)
      startRow--;
    else if (c == EAST)
      startCol++;
    else if (c == SOUTH)
      startRow++;
    else
      startCol--;
  }
}
