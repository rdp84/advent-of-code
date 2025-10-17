#include "day_14.h"

void part_1(void);
/* void part_2(void); */

int main(void)
{
  part_1();
  /* part_2(); */

  return EXIT_SUCCESS;
}

void part_1(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *tofree;

  robot *head, *tail;
  int xPos, yPos, xDir, yDir, i, j, tlQuadrant, trQuadrant, blQuadrant, brQuadrant, **map;
  int64_t safetyFactor;

  head = NULL;
  map = alloc_map();

  ifp = fopen("input.txt", "r");
  line = NULL;
  linecap = 0;
  xPos = yPos = xDir = yDir = 0;

  if ((linelen = getline(&line, &linecap, ifp)) > 0) {
    read_line(line, &xPos, &yPos, &xDir, &yDir);

    head = malloc(sizeof(head));
    head->xPos = xPos;
    head->yPos = yPos;
    head->xDir = xDir;
    head->yDir = yDir;
    tail = head;

    while ((linelen = getline(&line, &linecap, ifp)) > 0) {
      read_line(line, &xPos, &yPos, &xDir, &yDir);

      tail->next = malloc(sizeof(robot));
      tail = tail->next;
      tail->xPos = xPos;
      tail->yPos = yPos;
      tail->xDir = xDir;
      tail->yDir = yDir;
    }
    tail->next = NULL;
  }

  tail = head;
  while (tail != NULL) {
    find_end_pos(&(tail->xPos))

  }
    
    find_end_pos(&xPos, &yPos, xDir, yDir, 100);
    *(*(map + (-1 * yPos)) + xPos) += 1;

    tofree = line;
    line = NULL;
    free(tofree);
  }

  print_map(map);
  
  safetyFactor = 0;
  tlQuadrant = trQuadrant = 0;
  for (i = 0; i < (-1 * Y_BOUNDARY) / 2; i++) {
    for (j = 0; j < X_BOUNDARY / 2; j++)
      tlQuadrant += *(*(map + i) + j);
    for (j = (X_BOUNDARY / 2) + 1; j < X_BOUNDARY; j++)
      trQuadrant += *(*(map + i) + j);
  }

  blQuadrant = brQuadrant = 0;
  for (i = ((-1 * Y_BOUNDARY) / 2) + 1; i < -1 * Y_BOUNDARY; i++) {
    for (j = 0; j < X_BOUNDARY / 2; j++)
      blQuadrant += *(*(map + i) + j);
    for (j = (X_BOUNDARY / 2) + 1; j < X_BOUNDARY; j++)
      brQuadrant += *(*(map + i) + j);
  }

  safetyFactor = tlQuadrant * trQuadrant * blQuadrant * brQuadrant;
  printf("safety factor: %lld\n", safetyFactor);

  free_map(map);  
  fclose(ifp);
}

void part_2(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *tofree;

  int xPos, yPos, xDir, yDir, i, j, tlQuadrant, trQuadrant, blQuadrant, brQuadrant, **map;
  int64_t safetyFactor;

  map = alloc_map();

  ifp = fopen("input.txt", "r");
  line = NULL;
  linecap = 0;
  xPos = yPos = xDir = yDir = 0;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    read_line(line, &xPos, &yPos, &xDir, &yDir);
    find_end_pos(&xPos, &yPos, xDir, yDir, 100);
    *(*(map + (-1 * yPos)) + xPos) += 1;

    tofree = line;
    line = NULL;
    free(tofree);
  }

  print_map(map);
  
  safetyFactor = 0;
  tlQuadrant = trQuadrant = 0;
  for (i = 0; i < (-1 * Y_BOUNDARY) / 2; i++) {
    for (j = 0; j < X_BOUNDARY / 2; j++)
      tlQuadrant += *(*(map + i) + j);
    for (j = (X_BOUNDARY / 2) + 1; j < X_BOUNDARY; j++)
      trQuadrant += *(*(map + i) + j);
  }

  blQuadrant = brQuadrant = 0;
  for (i = ((-1 * Y_BOUNDARY) / 2) + 1; i < -1 * Y_BOUNDARY; i++) {
    for (j = 0; j < X_BOUNDARY / 2; j++)
      blQuadrant += *(*(map + i) + j);
    for (j = (X_BOUNDARY / 2) + 1; j < X_BOUNDARY; j++)
      brQuadrant += *(*(map + i) + j);
  }

  safetyFactor = tlQuadrant * trQuadrant * blQuadrant * brQuadrant;
  printf("safety factor: %lld\n", safetyFactor);

  free_map(map);  
  fclose(ifp);
}

int **alloc_map(void)
{
  int i, j, **map;

  map = malloc((-1 * Y_BOUNDARY) * sizeof(int *));
  for (i = 0; i < -1 * Y_BOUNDARY; i++)
    *(map + i) = malloc(X_BOUNDARY * sizeof(int));

  for (i = 0; i < -1 * Y_BOUNDARY; i++)
    for (j = 0; j < X_BOUNDARY; j++)
      *(*(map + i) + j) = 0;

  return map;
}

void free_map(int **map)
{
  int i;

  for (i = 0; i < -1 * Y_BOUNDARY; i++)
    free(*(map + i));
  free(map);
}

void print_map(int **map)
{
  int i, j;

  for (i = 0; i < -1 * Y_BOUNDARY; i++) {
    for (j = 0; j < X_BOUNDARY; j++) {
      if (*(*(map + i) + j) == 0)
        printf(".");
      else
        printf("%d", *(*(map + i) + j));
    }
    printf("\n");
  }
}


void read_line(char *line, int *xPos, int *yPos, int *xDir, int *yDir)
{
  int i, sign;

  for (i = 0; !isdigit(*(line + i)); i++)
    ;
  for (*xPos = 0; isdigit(*(line + i)); i++)
    *xPos = *xPos * 10 + (*(line + i) - '0');
  i++;

  for (*yPos = 0; isdigit(*(line + i)); i++)
    *yPos = *yPos * 10 + (*(line + i) - '0');
  *yPos *= -1;

  for (; !isdigit(*(line + i)) && *(line + i) != '-'; i++)
    ;
  sign = *(line + i) == '-' ? -1 : 1;
  if (*(line + i) == '-')
    i++;

  for (*xDir = 0; isdigit(*(line + i)); i++)
    *xDir = *xDir * 10 + (*(line + i) - '0');
  *xDir *= sign;
  i++;

  sign = *(line + i) == '-' ? 1 : -1;
  if (*(line + i) == '-')
    i++;

  for (*yDir = 0; isdigit(*(line + i)); i++)
    *yDir = *yDir * 10 + (*(line + i) - '0');
  *yDir *= sign;
}

void find_end_pos(int *xPos, int *yPos, int xDir, int yDir, int seconds)
{

  *xPos = (*xPos + (seconds * xDir)) % X_BOUNDARY;
  if (*xPos < 0)
    *xPos += X_BOUNDARY;
  
  *yPos = (*yPos + (seconds * yDir)) % Y_BOUNDARY;
  if (*yPos > 0)
    *yPos += Y_BOUNDARY;
}
