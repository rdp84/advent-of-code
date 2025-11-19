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
  bool foundStart, hasMoved;
  queue *q;
  location *allVisits;
  path p;

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
  p.score = 0;
  p.row = i;
  p.col = j;
  p.rowDir = 0;
  p.colDir = 1;
  p.visited = NULL;
  allVisits = NULL;


  enqueue(p, q);
  while (!empty(q)) {
    p = dequeue(q);
    if (*(*(map + p.row) + p.col) == END)
      break;
    else if (visited_score(p, allVisits) > p.score) {
      allVisits = add_location(allVisits, p);

      rotate(1, -1, p, map, q, allVisits); // clockwise
      rotate(-1, 1, p, map, q, allVisits); // counter-clockwise
      hasMoved = move(p, map, q, allVisits);
      
      if (!hasMoved)
        free_location(p.visited);
    }
    else {
      continue;
    }
  }
  /* print_location(allVisits); */
  /* print_queue(q); */
  printf("Found the end with a score of %d\n", p.score);

  add_path_to_map(p, rows, cols, map);
  /* print_map(map, rows, cols); */

  free_map(map, rows);
  free_queue(q);
  free_location(p.visited);
  free_location(allVisits);
}

void part_2(void)
{
  char **map;
  int i, j, rows, cols, best, bestScore, seats;
  bool foundStart, hasMoved;
  queue *q;
  location *allVisits, *l;
  path p, *bestPaths;

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
  p.score = 0;
  p.row = i;
  p.col = j;
  p.rowDir = 0;
  p.colDir = 1;
  p.visited = NULL;
  allVisits = NULL;

  best = 0;
  bestScore = INT32_MAX;
  bestPaths = NULL;
  
  enqueue(p, q);
  while (!empty(q)) {
    p = dequeue(q);
    /* printf("dequeued path with score %d, at (%d, %d), in direction (%d, %d)\n", p.score, p.row, p.col, p.rowDir, p.colDir); */
    
    if (p.score > bestScore) {
      printf("going to break with score %d, state of the queue:\n", p.score);
      /* print_queue(q); */
      break;
    }
    else if (*(*(map + p.row) + p.col) == END) {
      printf("Found a best path with a score of %d\n", p.score);
      bestScore = p.score;
      bestPaths = realloc(bestPaths, (best + 1) * sizeof(path));
      *(bestPaths + best) = p;
      best++;
    }
    else if (visited_score(p, allVisits) >= p.score) {
      allVisits = add_location(allVisits, p);

      rotate(1, -1, p, map, q, allVisits); // clockwise
      rotate(-1, 1, p, map, q, allVisits); // counter-clockwise
      hasMoved = move(p, map, q, allVisits);
      
      if (!hasMoved)
        free_location(p.visited);
    }
    else {
      /* printf("Moving onto next item in our queue\n"); */
      continue;
    }
  }

  /* print_location(allVisits); */
  printf("Found the end with a best score of %d\n", bestScore);
  printf("There were %d paths that had this score\n", best);

  i = best - 1;
  while (i >= 0) {
    p = *(bestPaths + i);
    l = p.visited;
    while (l != NULL) {
      *(*(map + l->row) + l->col) = 'O';
      l = l->next;
    }
    free_location(p.visited);
    i--;
  }
  free(bestPaths);

  seats = 0;
  for (i = 0; i < rows; i++)
    for (j = 0; j < cols; j++)
      if (*(*(map + i) + j) == 'O')
        seats++;

  printf("Number of seats %d\n", seats);
  print_map(map, rows, cols);
  free_map(map, rows);
  free_queue(q);
  free_location(allVisits);
}

void add_path_to_map(path p, int rows, int cols, char **map)
{
  location *l;

  l = p.visited;
  while (l != NULL) {
    if (l->rowDir == 0)
      if (l->colDir == 1)
        *(*(map + l->row) + l->col) = '>';
      else
        *(*(map + l->row) + l->col) = '<';
    else if (l->rowDir == -1)
      *(*(map + l->row) + l->col) = '^';
    else
      *(*(map + l->row) + l->col) = 'v';
    l = l->next;
  }
  print_map(map, rows, cols);

  l = p.visited;
  while (l != NULL) {
    if (l->rowDir == 0)
      if (l->colDir == 1)
        *(*(map + l->row) + l->col) = '.';
      else
        *(*(map + l->row) + l->col) = '.';
    else if (l->rowDir == -1)
      *(*(map + l->row) + l->col) = '.';
    else
      *(*(map + l->row) + l->col) = '.';
    l = l->next;
  }
  *(*(map + 1) + 15) = END;
}

bool move(path p, char **map, queue *q, location *allVisits)
{
  location *head;
  int newRow, newCol;
  
  newRow = p.row + p.rowDir;
  newCol = p.col + p.colDir;
  
  if (*(*(map + newRow) + newCol) != WALL) {
    p.score++;
    p.row = newRow;
    p.col = newCol;

    if (p.visited == NULL) {
      p.visited = alloc_location(p);
    } else {
      head = p.visited;
      while (head->next != NULL)
        head = head->next;
      head->next = alloc_location(p);
    }
    
    enqueue(p, q);

    return true;
  } else
    return false;
}

void rotate(int rowSign, int colSign, path p, char **map, queue *q, location *allVisits)
{
  location *head;
  int newRowDir, newColDir;

  newRowDir = rowSign * p.colDir;
  newColDir = colSign * p.rowDir;

  p.score += 1000;
  p.rowDir = newRowDir;
  p.colDir = newColDir;

  if (p.visited == NULL) {
    p.visited = alloc_location(p);
  } else {
    p.visited = copy_path_visits(p);
    head = p.visited;
    while (head->next != NULL)
      head = head->next;
    head->next = alloc_location(p);
  }
    
  enqueue(p, q);
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
  e->p.visited = p.visited;
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

  retValue.score = toFree->p.score;
  retValue.row = toFree->p.row;
  retValue.col = toFree->p.col;
  retValue.rowDir  = toFree->p.rowDir;
  retValue.colDir = toFree->p.colDir;
  retValue.visited = toFree->p.visited == NULL ? NULL : realloc(toFree->p.visited, sizeof(location));

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
    free_location(toFree->p.visited);
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
           e->p.row, e->p.col, e->p.score, e->p.rowDir, e->p.colDir);
    e = e->next;
  }
}

void print_location(location *l)
{
  while (l != NULL) {
    printf("location at (%d, %d) in direction (%d, %d) with a score of %d\n", l->row, l->col, l->rowDir, l->colDir, l->score);
    l = l->next;
  }

}

location *alloc_location(path p)
{
  location *retValue;

  retValue = malloc(sizeof(location));
  retValue->row = p.row;
  retValue->col = p.col;
  retValue->rowDir = p.rowDir;
  retValue->colDir = p.colDir;
  retValue->next = NULL;

  return retValue;
}

location *add_location(location *head, path p)
{
  location *l, *tail, *i;

  l = malloc(sizeof(location));
  l->score = p.score;
  l->row = p.row;
  l->col = p.col;
  l->rowDir = p.rowDir;
  l->colDir = p.colDir;
  l->next = NULL;
  
  if (head != NULL) {
    tail = i = head;

    while (i != NULL) {
      if (i->row == p.row && i->col == p.col && i->rowDir == p.rowDir && i->colDir == p.colDir) {
        /* printf("Found an existing location at (%d, %d) in direction (%d, %d) and a score %d. Path has score %d\n", i->row, i->col, i->rowDir, i->colDir, i->score, p.score); */
        i->score = p.score;
        free(l);
        return head;
      }
      i = i->next;
    }
    
    while (tail->next != NULL)
      tail = tail->next;
    tail->next = l;
  } else
    head = l;

  return head;
}

location *copy_path_visits(path p)
{
  location *head, *tail, *retValue;

  head = p.visited;
  if (head != NULL) {
    retValue = malloc(sizeof(location));
    retValue->row = head->row;
    retValue->col = head->col;
    retValue->rowDir = head->rowDir;
    retValue->colDir = head->colDir;
    retValue->next = NULL;

    tail = retValue;
    while ((head = head->next) != NULL) {
      tail->next = malloc(sizeof(location));
      tail = tail->next;
      tail->row = head->row;
      tail->col = head->col;
      tail->rowDir = head->rowDir;
      tail->colDir = head->colDir;
      tail->next = NULL;
    }
  } else
    retValue = NULL;

  return retValue;
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

int visited_score(path p, location *head)
{
  while (head != NULL) {
    if (head->row == p.row && head->col == p.col && head->rowDir == p.rowDir && head->colDir == p.colDir) {
      /* if (p.score < head->score) */
      /*   head->score = p.score; */
      
      return head->score;
    }
    head = head->next;
  }

  return INT32_MAX;
}

void remove_visits(path p, location *allVisits)
{
  location *pVisits, *head, *i, *j;

  head = allVisits;
  pVisits = p.visited;

  while (pVisits != NULL) {
    i = head;

    if (i->row == pVisits->row && i->col == pVisits->col &&
        i->rowDir == pVisits->rowDir && i->colDir == pVisits->colDir) {
      head = i->next;
      free(i);
    } else {
      j = i->next;

      while (j != NULL &&
             (j->row != pVisits->row || j->col != pVisits->col ||
              j->rowDir != pVisits->rowDir || j->colDir != pVisits->colDir)) {
        i = j;
        j = j->next;
      }

      if (j != NULL) {
        i->next = j->next;
        free(j);
      }
    }
    pVisits = pVisits->next;
  }
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
