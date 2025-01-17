#include "day_13.h"

void part_1(void);

int main(void)
{
  part_1();
  return EXIT_SUCCESS;
}


void part_1(void)
{
  FILE *ifp;
  char *line, *pos, *toFree, *num;
  size_t linecap;
  ssize_t linelen;
  int numlen, x, y, i;

  ifp = fopen("example_input.txt", "r");
  line = NULL;
  linecap = 0;
  i = 1;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;
    if (*line != '\n') {
      while (*line++ != 'X')
        ;

      line++;
      pos = line;
      while (*line++ != ',')
        ;

      numlen = line - pos;
      num = malloc(numlen * sizeof(char));
      strlcpy(num, pos, numlen);
      x = atoi(num);
      free(num);

      while (*line++ != 'Y')
        ;

      line++;
      pos = line;
      while (*line++ != '\n')
        ;

      numlen = line - pos;
      num = malloc(numlen * sizeof(char));
      strlcpy(num, pos, numlen);
      y = atoi(num);
      free(num);

      if (i == 1)
        printf("Button A: X+%d, Y+%d\n", x, y);
      else if (i == 2)
        printf("Button B: X+%d, Y+%d\n", x, y);
      else
        printf("Prize: X=%d, Y=%d\n", x, y);

      i++;
    }
    else
      i = 1;
    line = NULL;
    free(toFree);
  }
  
  fclose(ifp);
}
