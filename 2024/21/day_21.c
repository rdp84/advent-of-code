#include "day_21.h"

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
  int  depth;
  long total;

  depth = 2;
  total = find_cost(depth);
  printf("depth: %d, total: %ld\n", depth, total);
}

void part_2(void)
{
  int  depth;
  long total;

  depth = 25;
  total = find_cost(depth);
  printf("depth: %d, total: %ld\n", depth, total);
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
  }
  else
    q->front = q->rear = e;

  q->cnt++;
}

path dequeue(queue *q)
{
  elem *toFree;
  path  retValue;

  toFree = q->front;
  retValue = toFree->p;
  q->front = q->front->next;
  q->cnt--;
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

  printf("queue has %d elem\n", q->cnt);

  e = q->front;
  while (e != NULL) {

    if (e->p.taken != NULL)
      printf("elem with score %d, at position (%d, %d), moving in direction %c"
             " has made steps %d and followed %s\n",
             e->p.score, e->p.row, e->p.col, e->p.dir, e->p.score, e->p.taken);
    else
      printf("elem with score %d, at position (%d, %d), moving in direction %c"
             " has made steps %d and followed NULL\n",
             e->p.score, e->p.row, e->p.col, e->p.dir, e->p.score);
    e = e->next;
  }
}

/* Seen functions */

location ***alloc_seen(int rows, int cols)
{
  int i, j;
  location ***retValue;

  retValue = malloc(rows * sizeof(location *));
  for (i = 0; i < rows; i++) {
    *(retValue + i) = malloc(cols * sizeof(location *));
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

/* Movement functions */

void rotate(path p, queue *q)
{
  char *currentTaken;

  currentTaken = p.taken;

  if (currentTaken != NULL) {
    p.taken = malloc((p.score + 2) * sizeof(char));
    p.taken = strcpy(p.taken, currentTaken);
  }
  else
    p.taken = NULL;

  move(p, q);
}

void rotate_one_eighty(path p, queue *q)
{
  if (p.dir == UP)
    p.dir = DOWN;
  else if (p.dir == DOWN)
    p.dir = UP;
  else if (p.dir == RIGHT)
    p.dir = LEFT;
  else
    p.dir = RIGHT;

  rotate(p, q);
}

void rotate_ninety_clockwise(path p, queue *q)
{
  if (p.dir == UP)
    p.dir = RIGHT;
  else if (p.dir == DOWN)
    p.dir = LEFT;
  else if (p.dir == RIGHT)
    p.dir = DOWN;
  else
    p.dir = UP;

  rotate(p, q);
}

void rotate_ninety_counter_clockwise(path p, queue *q)
{
  if (p.dir == UP)
    p.dir = LEFT;
  else if (p.dir == DOWN)
    p.dir = RIGHT;
  else if (p.dir == RIGHT)
    p.dir = UP;
  else
    p.dir = DOWN;

  rotate(p, q);
}

void move(path p, queue *q)
{
  int newRow, newCol;

  newRow = p.row;
  newCol = p.col;

  if (p.dir == UP)
    newRow--;
  else if (p.dir == DOWN)
    newRow++;
  else if (p.dir == RIGHT)
    newCol++;
  else
    newCol--;

  p.score++;
  p.row = newRow;
  p.col = newCol;

  p.taken = realloc(p.taken, (p.score + 2) * sizeof(char));
  *(p.taken + p.score - 1) = p.dir;
  *(p.taken + p.score) = ACTIVATE;
  *(p.taken + p.score + 1) = '\0';

  enqueue(p, q);
}

/* Pad functions */

char **alloc_numpad(void)
{
  char **retValue;

  retValue = malloc(NUMERICAL_PAD_ROWS * sizeof(char *));
  *(retValue) = strdup("789");
  *(retValue + 1) = strdup("456");
  *(retValue + 2) = strdup("123");
  *(retValue + 3) = strdup(" 0A");

  return retValue;
}

char **alloc_dirpad(void)
{
  char **retValue;

  retValue = malloc(DIRECTION_PAD_ROWS * sizeof(char *));
  *(retValue) = strdup(" ^A");
  *(retValue + 1) = strdup("<v>");

  return retValue;
}

void print_pad(char **pad, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    printf("%s\n", *(pad + i));
}

void free_pad(char **pad, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    free(*(pad + i));
  free(pad);
}

/* Path functions */

best ****alloc_pad_paths(int rows, int cols)
{
  int i, j, ii, jj;
  best ****retValue;

  retValue = malloc(rows * sizeof(best *));
  for (i = 0; i < rows; i++) {
    *(retValue + i) = malloc(cols * sizeof(best *));
    for (j = 0; j < cols; j++) {
      *(*(retValue + i) + j) = malloc(rows * sizeof(best *));
      for (ii = 0; ii < rows; ii++) {
        *(*(*(retValue + i) + j) + ii) = malloc(cols * sizeof(best));
        for (jj = 0; jj < cols; jj++) {
          (*(*(*(retValue + i) + j) + ii) + jj)->cnt = 0;
          (*(*(*(retValue + i) + j) + ii) + jj)->paths = NULL;
        }
      }
    }
  }
  return retValue;
}

void init_pad_paths(char **pad, int rows, int cols, best ****paths)
{
  int i, j, ii, jj, bestScore;
  location ***seen;
  queue *q;
  path p;
  best *b;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      if (*(*(pad + i) + j) != SPACE) {
        for (ii = 0; ii < rows; ii++) {
          for (jj = 0; jj < cols; jj++) {
            if (*(*(pad + ii) + jj) != SPACE) {
              b = (*(*(*(paths + i) + j) + ii) + jj);
              bestScore = INT32_MAX;

              seen = alloc_seen(rows,
                                cols);

              q = alloc_queue();
              p.score = 0;
              p.row = i;
              p.col = j;
              p.dir = RIGHT;
              p.taken = malloc(2 * sizeof(char));
              *(p.taken) = ACTIVATE;
              *(p.taken + 1) = '\0';

              enqueue(p, q);
              while (!(empty(q))) {
                p = dequeue(q);

                if (p.score > bestScore)
                  break;
                else if (p.row == ii && p.col == jj) {
                  bestScore = p.score;
                  b->paths = realloc(b->paths,
                                    (b->cnt + 1) * sizeof(char *));
                  *(b->paths + b->cnt) = p.taken;
                  b->cnt++;
                } else if (p.row < 0 || p.row >= rows ||
                           p.col < 0 || p.col >= cols ||
                           *(*(pad + p.row) + p.col) == SPACE ||
                           p.score > seen_score(seen, p))
                  free(p.taken);
                else {
                  rotate_one_eighty(p, q);
                  rotate_ninety_clockwise(p, q);
                  rotate_ninety_counter_clockwise(p, q);
                  move(p, q);
                }
              }
              free_seen(seen, rows, cols);
              free_queue(q);
              seen = NULL;
              q = NULL;
            }
          }
        }
      }
    }
  }
}

void print_pad_paths(best ****paths, int rows, int cols)
{
  int i, ii, j, jj, k;
  best *b;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      for (ii = 0; ii < rows; ii++) {
        for (jj = 0; jj < cols; jj++) {
          b = (*(*(*(paths + i) + j) + ii) + jj);
          printf("Found %d paths from (%d, %d) to (%d, %d):\n", b->cnt, i, j, ii, jj);
          if (b->paths == NULL)
            printf("NULL\n");
          else
            for (k = 0; k < b->cnt; k++)
              printf("%s\n", *(b->paths + k));
        }
      }
    }
  }
}

void free_pad_paths(best ****paths, int rows, int cols)
{
  int i, ii, j, jj, k;
  best *b;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      for (ii = 0; ii < rows; ii++) {
        for (jj = 0; jj < cols; jj++) {
          b = (*(*(*(paths + i) + j) + ii) + jj);
          if (b->paths != NULL) {
            for (k = 0; k < b->cnt; k++)
              free(*(b->paths + k));
          }
        }
        free(*(*(*(paths + i) + j) + ii));
      }
      free(*(*(paths + i) + j));
    }
    free(*(paths + i));
  }
  free(paths);
}

/* Cache functions */

entry **alloc_cache(int size)
{
  int i;
  entry **retValue;

  retValue = malloc(size * sizeof(entry *));
  for (i = 0; i < size; i++)
    *(retValue + i) = NULL;

  return retValue;
}

void print_cache(entry **cache, int size)
{
  int i;
  entry *e;

  for (i = 0; i < size; i++) {
    printf("cache at depth %d has the following entries:\n", i);
    e = *(cache + i);
    while (e != NULL) {
      printf("code: %s, cost: %lu\n", e->code, e->cost);
      e = e->next;
    }
  }
}

long get_cost(entry **cache, int depth, char *code)
{
  entry *e;
  long retValue;

  retValue = -1;
  e = *(cache + depth);

  while (e != NULL) {
    if (strcmp(e->code, code) == 0) {
      retValue = e->cost;
      break;
    }
    else
      e = e->next;
  }

  return retValue;
}

void add_cost(entry **cache, int depth, char *code, long cost)
{
  entry *e, *next;

  e = *(cache + depth);
  if (e == NULL) {
    e = malloc(sizeof(entry));
    e->code = code;
    e->cost = cost;
    e->next = NULL;
    *(cache + depth) = e;
  } else {
    next = e->next;
    while (next != NULL) {
      e = e->next;
      next = next->next;
    }
    e->next = malloc(sizeof(entry));
    e->next->code = code;
    e->next->cost = cost;
    e->next->next = NULL;
  }
}

void free_cache(entry **cache, int size)
{
  int i;
  entry *e, *toFree;

  for (i = 0; i < size; i++) {
    e = *(cache + i);
    while (e != NULL) {
      toFree = e;
      e = e->next;
      free(toFree);
    }
  }
  free(cache);
}

/* Code functions */

char **alloc_start_codes(int *codeslen)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  retValue = NULL;
  *codeslen = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    retValue = realloc(retValue, (*codeslen + 1) * sizeof(char *));
    *(line + linelen - 1) = '\0';
    *(retValue + *codeslen) = line;
    (*codeslen)++;
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

void print_start_codes(char **codes, int len)
{
  int i;

  for (i = 0; i < len; i++)
    printf("%s\n", *(codes + i));
}

void free_start_codes(char **codes, int len)
{
  int i;

  for (i = 0; i < len; i++)
    free(*(codes + i));
  free(codes);
}

char *prepend_activate(char *code)
{
  int len;
  char *retValue;

  len = strlen(code) + 2;
  retValue = malloc(len * sizeof(char));
  *retValue = ACTIVATE;
  *(retValue + 1) = '\0';
  (void) strcat(retValue, code);

  return retValue;
}

long numeric_code(char *code)
{
  char c;
  long retValue;

  retValue = 0;
  while ((c = *code++) != ACTIVATE)
    retValue = 10 * retValue + (c - '0');

  return retValue;
}

void find_num_index(char c, int *i, int *j)
{
  switch (c) {
  case ACTIVATE:
    *i = 3;
    *j = 2;
    break;
  case ZERO:
    *i = 3;
    *j = 1;
    break;
  case ONE:
    *i = 2;
    *j = 0;
    break;
  case TWO:
    *i = 2;
    *j = 1;
    break;
  case THREE:
    *i = *j = 2;
    break;
  case FOUR:
    *i = 1;
    *j = 0;
    break;
  case FIVE:
    *i = *j = 1;
    break;
  case SIX:
    *i = 1;
    *j = 2;
    break;
  case SEVEN:
    *i = *j = 0;
    break;
  case EIGHT:
    *i = 0;
    *j = 1;
    break;
  case NINE:
    *i = 0;
    *j = 2;
    break;
  default:
    printf("error: unknown num %c\n", c);
    *i = 3;
    *j = 0;
    break;
  }
}

void find_dir_index(char c, int *i, int *j)
{
  switch (c) {
  case ACTIVATE:
    *i = 0;
    *j = 2;
    break;
  case UP:
    *i = 0;
    *j = 1;
    break;
  case LEFT:
    *i = 1;
    *j = 0;
    break;
  case DOWN:
    *i = *j = 1;
    break;
  case RIGHT:
    *i = 1;
    *j = 2;
    break;
  default:
    printf("error: unknown dir %c\n", c);
    *i = *j = 0;
    break;
  }
}

/* Cost functions */

long find_cost(int depth)
{
  char **codes, **numpad, **dirpad;
  int i, codeslen;
  long cost, numeric, retValue;
  best ****numpadpaths, ****dirpadpaths;
  entry **cache;

  numpad = alloc_numpad();
  dirpad = alloc_dirpad();

  numpadpaths = alloc_pad_paths(NUMERICAL_PAD_ROWS, NUMERICAL_PAD_COLS);
  init_pad_paths(numpad, NUMERICAL_PAD_ROWS, NUMERICAL_PAD_COLS, numpadpaths);

  dirpadpaths = alloc_pad_paths(DIRECTION_PAD_ROWS, DIRECTION_PAD_COLS);
  init_pad_paths(dirpad, DIRECTION_PAD_ROWS, DIRECTION_PAD_COLS, dirpadpaths);

  cache = NULL;
  cost = retValue = 0;
  codeslen = 0;
  codes = alloc_start_codes(&codeslen);
  for (i = 0; i < codeslen; i++) {
    cache = alloc_cache(depth);
    cost = find_numpad_cost(*(codes + i), depth, numpadpaths, dirpadpaths, cache);
    numeric = numeric_code(*(codes + i));
    retValue += cost * numeric;

    free_cache(cache, depth);
    cache = NULL;
  }

  free_pad(numpad, NUMERICAL_PAD_ROWS);
  free_pad(dirpad, DIRECTION_PAD_ROWS);
  free_pad_paths(numpadpaths, NUMERICAL_PAD_ROWS, NUMERICAL_PAD_COLS);
  free_pad_paths(dirpadpaths, DIRECTION_PAD_ROWS, DIRECTION_PAD_COLS);
  free_start_codes(codes, codeslen);

  return retValue;
}

long find_numpad_cost(char *code, int depth, best ****numpadpaths, best ****dirpadpaths, entry **cache)
{
  char from, to, *acode, **paths;
  int i, j, codelen, x1, y1, x2, y2;
  long retValue, *costs;
  best *b;

  retValue = 0;
  acode = prepend_activate(code);
  codelen = strlen(acode);
  for (i = 0; i < codelen - 1; i++) {
    x1 = y1 = x2 = y2 = 0;

    from = *(acode + i);
    find_num_index(from, &x1, &y1);

    to = *(acode + i + 1);
    find_num_index(to, &x2, &y2);

    b = (*(*(*(numpadpaths + x1) + y1) + x2) + y2);
    paths = b->paths;

    if (depth == 0)
      retValue += (long) strlen(*paths);
    else {
      costs = malloc(b->cnt * sizeof(long));
      for (j = 0; j < b->cnt; j++)
        *(costs + j) = find_dirpad_cost(*(paths + j), depth - 1, dirpadpaths, cache);
      retValue += min_cost(costs, b->cnt);
      free(costs);
    }
  }
  free(acode);
  return retValue;
}

long find_dirpad_cost(char *code, int depth, best ****dirpadpaths, entry **cache)
{
  char from, to, *acode, **paths;
  int i, j, codelen, x1, y1, x2, y2;
  long cost, *costs;
  best *b;

  if ((cost = get_cost(cache, depth, code)) != -1)
    return cost;
  else {
    cost = 0;
    acode = prepend_activate(code);
    codelen = strlen(acode);
    for (i = 0; i < codelen - 1; i++) {
      x1 = y1 = x2 = y2 = 0;

      from = *(acode + i);
      find_dir_index(from, &x1, &y1);

      to = *(acode + i + 1);
      find_dir_index(to, &x2, &y2);

      b = (*(*(*(dirpadpaths + x1) + y1) + x2) + y2);
      paths = b->paths;

      if (depth == 0)
        cost += (long) strlen(*paths);
      else {
        costs = malloc(b->cnt * sizeof(long));
        for (j = 0; j < b->cnt; j++)
          *(costs + j) = find_dirpad_cost(*(paths + j), depth - 1, dirpadpaths, cache);
        cost += min_cost(costs, b->cnt);
        free(costs);
      }
    }
    add_cost(cache, depth, code, cost);
    free(acode);
    return cost;
  }
}

long min_cost(long *costs, int len)
{
  int i;
  long retValue;

  retValue = INT64_MAX;
  for (i = 0; i < len; i++)
    if (*(costs + i) < retValue)
      retValue = *(costs + i);

  return retValue;
}
