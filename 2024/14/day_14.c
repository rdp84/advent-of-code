#include "day_14.h"

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
  robot *head, *tail;
  int i, j, tlQuadrant, trQuadrant, blQuadrant, brQuadrant, **map;
  int64_t safetyFactor;

  head = alloc_robots();
  map = alloc_map();

  tail = head;
  while (tail != NULL) {
    find_end_pos(tail, 100);
    tail = tail->next;
  }

  draw_map(map, head);
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

  free_robots(head);
  free_map(map);  
}

void part_2(void)
{
  robot *head, *tail;
  int cluster, **map;
  int64_t seconds;
  char input[3];

  head = alloc_robots();
  map = alloc_map();
  seconds = cluster = 0;

  printf("%s",
         "Beginning search for the Christmas tree\n"
         "To continue searching, hit Enter or type 'c'\n"
         "To quit, Ctrl+d or type 'q'\n");

  while (get_user_input(input) > 0) {
    if (input[0] == '\n' ||
        (input[0] == 'c' && input[1] == '\n')) {

      while (cluster == 0) {
        tail = head;
        seconds++;

        while (tail != NULL) {
          find_end_pos(tail, 1);
          tail = tail->next;
        }
        draw_map(map, head);
        cluster = find_cluster(map);
        tail = head;
      }
      printf("Possibly found a cluster after %lld seconds\n", seconds);
      print_map(map);
      cluster = 0;

      printf("%s",
             "To continue searching, hit Enter or type 'c'\n"
             "To quit, Ctrl+d or type 'q'\n");
    } else if (input[0] == 'q' && input[1] == '\n')
      break;
    else
      printf("Input not recognised, try again\n");
  }

  free_robots(head);
  free_map(map);
  printf("Quitting after %lld seconds\n", seconds);
}

robot *alloc_robots(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *toFree;

  robot *head, *tail;
  int xPos, yPos, xDir, yDir;

  ifp = fopen("input.txt", "r");
  line = NULL;
  linecap = 0;

  head = NULL;
  xPos = yPos = xDir = yDir = 0;

  if ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;
    read_line(line, &xPos, &yPos, &xDir, &yDir);
    free(toFree);
    line = NULL;
    
    head = malloc(sizeof(robot));
    head->xPos = xPos;
    head->yPos = yPos;
    head->xDir = xDir;
    head->yDir = yDir;
    tail = head;

    while ((linelen = getline(&line, &linecap, ifp)) > 0) {
      toFree = line;
      read_line(line, &xPos, &yPos, &xDir, &yDir);

      tail->next = malloc(sizeof(robot));
      tail = tail->next;
      tail->xPos = xPos;
      tail->yPos = yPos;
      tail->xDir = xDir;
      tail->yDir = yDir;

      free(toFree);
      line = NULL;
    }
    tail->next = NULL;
  }

  fclose(ifp);
  return head;
}

void free_robots(robot *robots)
{
  robot *toFree;

  while (robots != NULL) {
    toFree = robots;
    robots = robots->next;
    free(toFree);
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

void find_end_pos(robot *r, int seconds)
{
  r->xPos = (r->xPos + (seconds * r->xDir)) % X_BOUNDARY;
  if (r->xPos < 0)
    r->xPos += X_BOUNDARY;
  
  r->yPos = (r->yPos + (seconds * r->yDir)) % Y_BOUNDARY;
  if (r->yPos > 0)
    r->yPos += Y_BOUNDARY;
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

void draw_map(int **map, robot *robots)
{
  int i, j;

  for (i = 0; i < -1 * Y_BOUNDARY; i++)
    for (j = 0; j < X_BOUNDARY; j++)
      *(*(map + i) + j) = 0;
  
  while (robots != NULL) {
    *(*(map + (-1 * robots->yPos)) + robots->xPos) += 1;
    robots = robots->next;
  }
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

int get_user_input(char line[])
{
  char c;
  int i;

  i = 0;
  while (i < 2 && ((c = getchar()) != EOF) && c != '\n') {
    if (c != ' ' && c != '\t')
      line[i++] = c;
  }

  if (c != EOF) {
    if (c == '\n')
      line[i++] = c;
    else
      while ((c = getchar()) != '\n')
        ;
  }

  line[i] = '\0';
  return i;
}

int find_cluster(int **map)
{
  int i, j;

  for (i = 0; i < (-1 * Y_BOUNDARY) - 4; i++) {
    for (j = 0; j < X_BOUNDARY - 1; j++) {
      if (*(*(map + i) + j) > 0 &&
          *(*(map + i + 1) + j) > 0 &&
          *(*(map + i + 2) + j) > 0 &&
          *(*(map + i + 3) + j) > 0 &&
          *(*(map + i + 4) + j) > 0 &&
          *(*(map + i) + j + 1) > 0 &&
          *(*(map + i + 1) + j + 1) > 0 &&
          *(*(map + i + 2) + j + 1) > 0 &&
          *(*(map + i + 3) + j + 1) > 0 &&
          *(*(map + i + 4) + j + 1) > 0) {
        return 1;
      }
    }
  }
  return 0;
}
