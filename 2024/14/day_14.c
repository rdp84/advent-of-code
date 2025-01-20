#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

/* a = -203; */
/* b = -7; */
/* printf("a / b = %d, a %c b = %d\n", a / b, '%', a % b); */

#define X_BOUNDARY 11
#define Y_BOUNDARY -7
#define SECONDS    100

void find_robots(void);
void read_line(char *, int *, int *, int *, int *);
void find_end_pos(int *, int *, int, int);

int main(void)
{
  find_robots();

  return EXIT_SUCCESS;
}


void find_robots(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *tofree;

  int xPos, yPos, xDir, yDir, i, j, map[-1 * Y_BOUNDARY][X_BOUNDARY];
  
  ifp = fopen("day_14_input.txt", "r");
  line = NULL;
  linecap = 0;
  xPos = yPos = xDir = yDir = 0;

  for (i = 0; i < -1 * Y_BOUNDARY; i++)
    for (j = 0; j < X_BOUNDARY; j++)
      map[i][j] = 0;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    printf("%s", line);
    read_line(line, &xPos, &yPos, &xDir, &yDir);
    printf("start pos: (%d, %d), vector: (%d, %d)\n", xPos, yPos, xDir, yDir);
    find_end_pos(&xPos, &yPos, xDir, yDir);
    printf("final pos: (%d, %d)\n", xPos, yPos);
    map[-1 * yPos][xPos] += 1;
    tofree = line;
    line = NULL;
    free(tofree);
  }

  for (i = 0; i < -1 * Y_BOUNDARY; i++) {
    for (j = 0; j < X_BOUNDARY; j++) {
      if (map[i][j] == 0)
        printf(".");
      else
        printf("%d", map[i][j]);
    }
    printf("\n");
  }
      
      
  fclose(ifp);
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

void find_end_pos(int *xPos, int *yPos, int xDir, int yDir)
{

  *xPos = (*xPos + (SECONDS * xDir)) % X_BOUNDARY;
  if (*xPos < 0)
    *xPos += X_BOUNDARY;
  
  *yPos = (*yPos + (SECONDS * yDir)) % Y_BOUNDARY;
  if (*yPos > 0)
    *yPos += Y_BOUNDARY;
}
