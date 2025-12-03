#include "day_18.h"

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
  coord *coords;
  int coordsLen;
  char **mem;
  location ***seen;
  queue *q;
  path p;

  coordsLen = 0;
  coords = alloc_coords(&coordsLen);
  mem = alloc_mem_space();
  add_bytes(coords, mem, NANO_SEC - 1);
  print_mem_space(mem);

  seen = alloc_seen();

  q = alloc_queue();
  p.score = p.takenPos = 0;
  p.x = p.y = 0;
  p.dir = RIGHT;
  p.taken = NULL;

  enqueue(p, q);
  while (!empty(q)) {
    p = dequeue(q);

    if ((p.x == X_SIZE - 1) && (p.y == Y_SIZE - 1)) {
      printf("Found end with score %d and path taken:\n%s\n\n", p.score, p.taken);
      add_taken_to_mem(p.taken, mem);
      print_mem_space(mem);
      free(p.taken);
      break;
    }
    else if (p.score < seen_score(seen, p)) {
      rotate_ninety_clockwise(p, q);
      rotate_ninety_counter_clockwise(p, q);
      rotate_one_eighty(p, q);
      move(p, mem, q);
    }
    else
      free(p.taken);
  }

  free_coords(coords);
  free_mem_space(mem);
  free_seen(seen);
  free_queue(q);
}

void part_2(void)
{
  coord *coords;
  int coordsLen, successPos, failPos, testPos;
  bool foundPath;
  char **mem;
  location ***seen;
  queue *q;
  path p;

  coordsLen = 0;
  coords = alloc_coords(&coordsLen);
  mem = alloc_mem_space();

  successPos = NANO_SEC - 1;
  failPos = -1;
  testPos = coordsLen - 1;

  while (failPos - successPos != 1) {
    add_bytes(coords, mem, testPos);

    foundPath = false;
    seen = alloc_seen();
    q = alloc_queue();
    p.score = p.takenPos = 0;
    p.x = p.y = 0;
    p.dir = RIGHT;
    p.taken = NULL;

    enqueue(p, q);
    while (!empty(q)) {
      p = dequeue(q);

      if ((p.x == X_SIZE - 1) && (p.y == Y_SIZE - 1)) {
        free(p.taken);
        foundPath = true;
        break;
      }
      else if (p.score < seen_score(seen, p)) {
        rotate_ninety_clockwise(p, q);
        rotate_ninety_counter_clockwise(p, q);
        rotate_one_eighty(p, q);
        move(p, mem, q);
      }
      else
        free(p.taken);
    }

    if (foundPath) {
      successPos = testPos;
      testPos += ((coordsLen - 1 - testPos) / 2);
    }
    else {
      failPos = testPos;
      testPos -= ((testPos - successPos) / 2);
    }

    clear_bytes(mem);
    free_seen(seen);
    free_queue(q);
  }

  printf("successPos: %d, coords: (%d, %d)\n", successPos, (*(coords + successPos)).x , (*(coords + successPos)).y);
    printf("failPos: %d, coords: (%d, %d)\n", failPos, (*(coords + failPos)).x , (*(coords + failPos)).y);
  printf("testPos: %d, coords (%d, %d)\n", testPos, (*(coords + testPos)).x, (*(coords + testPos)).y);
  free_coords(coords);
  free_mem_space(mem);
}


coord *alloc_coords(int *len)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line;
  int i, x, y, n;
  coord *retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  n = 0;
  retValue = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    i = x = y = 0;
    n++;

    while (isdigit(*(line + i))) {
      x = 10 * x + (*(line + i) - '0');
      i++;
    }
    i++;
    while (isdigit(*(line + i))) {
      y = 10 * y + (*(line + i) - '0');
      i++;
    }

    retValue = realloc(retValue, n * sizeof(coord));
    (*(retValue + n - 1)).x = x;
    (*(retValue + n - 1)).y = y;

    free(line);
    line = NULL;
  }

  *len = n;
  return retValue;
}

char **alloc_mem_space(void)
{
  int i, j;
  char **retValue;

  retValue = malloc(Y_SIZE * sizeof(char *));
  for (i = 0; i < Y_SIZE; i++)
    *(retValue + i) = malloc(X_SIZE * sizeof(char));

  for (i = 0; i < Y_SIZE; i++)
    for (j = 0; j < X_SIZE; j++)
      *(*(retValue + i) + j) = SAFE;

  return retValue;
}

void add_bytes(coord *coords, char **mem, int lim)
{
  int i;
  coord c;

  for (i = 0; i <= lim; i++) {
    c = *(coords + i);
    *(*(mem + c.y) + c.x) = CORRUPTED;
  }
}

void clear_bytes(char **mem)
{
  int i, j;

  for (i = 0; i < Y_SIZE; i++)
    for (j = 0; j < X_SIZE; j++)
      *(*(mem + i) + j) = SAFE;
}

void free_coords(coord *coords)
{
  free(coords);
}

void free_mem_space(char **mem)
{
  int i;

  for (i = 0; i < X_SIZE; i++)
    free(*(mem + i));

  free(mem);
}

void print_coords(coord *coords, int len)
{
  int i;

  for (i = 0; i < len; i++)
    printf("x: %d, y: %d\n", (*(coords + i)).x, (*(coords + i)).y);
}

void print_mem_space(char **mem)
{
  int i, j;

  for (i = 0; i < Y_SIZE; i++) {
    for (j = 0; j < X_SIZE; j++)
      printf("%c", *(*(mem + i) + j));
    printf("\n");
  }
}

/* Seen functions */

location ***alloc_seen(void)
{
  int i, j;
  location ***retValue;

  retValue = calloc(Y_SIZE, sizeof(location *));
  for (i = 0; i < Y_SIZE; i++) {
    *(retValue + i) = calloc(X_SIZE, sizeof(location *));
    for (j = 0; j < X_SIZE; j++)
      *(*(retValue + i) + j) = NULL;
  }

  return retValue;
}

int seen_score(location ***seen, path p)
{
  location *current, *next, *toAdd;
  int retScore;

  current = *(*(seen + p.y) + p.x);

  if (current == NULL) {
    current = malloc(sizeof(location));
    current->score = p.score;
    current->x = p.x;
    current->y = p.y;
    current->dir = p.dir;
    current->next = NULL;
    *(*(seen + p.y) + p.x) = current;

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
    toAdd->x = p.x;
    toAdd->y = p.y;
    toAdd->dir = p.dir;
    toAdd->score = p.score;
    toAdd->next = NULL;
    current->next = toAdd;

    return INT32_MAX;
  }
}

void free_seen(location ***seen)
{
  int i, j;

  for (i = 0; i < Y_SIZE; i++) {
    for (j = 0; j < X_SIZE; j++)
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
           l->x, l->y, l->dir, l->score);
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
  retValue.x = toFree->p.x;
  retValue.y = toFree->p.y;
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
      while ((c = *(e->p.taken + i)) == UP || c == DOWN || c == LEFT || c == RIGHT)
        i++;
      *(e->p.taken + i) = '\0';
      printf("elem at (%d, %d), with score %d, moving in direction %c, has completed "\
             "the following %d steps: %s\n",
             e->p.x, e->p.y, e->p.score, e->p.dir, e->p.takenPos, e->p.taken);
      *(e->p.taken + i) = c;
    }
    else
      printf("elem at (%d, %d), with score %d, moving in direction %c, has completed "\
             "the following %d steps: NULL\n",
             e->p.x, e->p.y, e->p.score, e->p.dir, e->p.takenPos);

    e = e->next;
  }
}

/* Movement functions */

void rotate(path p, queue *q)
{
  char *currentTaken;

  currentTaken = p.taken;

  if (currentTaken != NULL) {
    p.taken = malloc_taken(p);
    p.taken = strcpy(p.taken, currentTaken);
  }
  else
    p.taken = NULL;

  enqueue(p, q);
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

void move(path p, char **mem, queue *q)
{
  int newX, newY;

  newX = p.x;
  newY = p.y;

  if (p.dir == UP)
    newY--;
  else if (p.dir == DOWN)
    newY++;
  else if (p.dir == RIGHT)
    newX++;
  else
    newX--;

  if (in_bounds(newX, newY) &&
      *(*(mem + newY) + newX) != CORRUPTED) {
    p.score++;
    p.x = newX;
    p.y = newY;

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

void add_taken_to_mem(char *taken, char **mem)
{
  int x, y;
  char c;

  x = y = 0;
  while ((c = *taken++) == UP || c == DOWN || c == LEFT || c == RIGHT) {
    *(*(mem + y) + x) = c;

    if (c == UP)
      y--;
    else if (c == DOWN)
      y++;
    else if (c == LEFT)
      x--;
    else
      x++;
  }
}
