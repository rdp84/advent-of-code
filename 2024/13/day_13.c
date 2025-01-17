#include "day_13.h"

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
  FILE *ifp;
  char *line, *pos, *toFree, *num;
  size_t linecap;
  ssize_t linelen;
  int numlen, x, y, i;
  int64_t  tokens, pushes[2];
  equation xTerms, yTerms;

  ifp = fopen("input.txt", "r");
  line = NULL;
  linecap = 0;
  tokens = 0;
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

      if (i == 1) {
        printf("Button A: X+%d, Y+%d\n", x, y);
        xTerms.a = x;
        yTerms.a = y;
      }
      else if (i == 2) {
        printf("Button B: X+%d, Y+%d\n", x, y);
        xTerms.b = x;
        yTerms.b = y;
      }
      else {
        printf("Prize: X=%d, Y=%d\n", x, y);
        xTerms.equals = x;
        yTerms.equals = y;
        pushes[0] = pushes[1] = 0;
        solve_sim_equation(xTerms, yTerms, pushes);
        printf("number of a pushes: %lld, number of b pushes: %lld\n", pushes[0], pushes[1]);
        tokens += (pushes[0] * A_TOKENS) + (pushes[1] * B_TOKENS);
      }

      i++;
    }
    else
      i = 1;
    line = NULL;
    free(toFree);
  }
  printf("tokens: %lld\n", tokens);
  fclose(ifp);
}

void part_2(void)
{
  FILE *ifp;
  char *line, *pos, *toFree, *num;
  size_t linecap;
  ssize_t linelen;
  int numlen, x, y, i;
  int64_t tokens, pushes[2];
  equation xTerms, yTerms;

  ifp = fopen("input.txt", "r");
  line = NULL;
  linecap = 0;
  tokens = 0;
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

      if (i == 1) {
        printf("Button A: X+%d, Y+%d\n", x, y);
        xTerms.a = x;
        yTerms.a = y;
      }
      else if (i == 2) {
        printf("Button B: X+%d, Y+%d\n", x, y);
        xTerms.b = x;
        yTerms.b = y;
      }
      else {
        printf("Prize: X=%d, Y=%d\n", x, y);
        xTerms.equals = x + CORRECTION;
        yTerms.equals = y + CORRECTION;
        pushes[0] = pushes[1] = 0;
        solve_sim_equation(xTerms, yTerms, pushes);
        printf("number of a pushes: %lld, number of b pushes: %lld\n", pushes[0], pushes[1]);
        tokens += (pushes[0] * A_TOKENS) + (pushes[1] * B_TOKENS);
      }

      i++;
    }
    else
      i = 1;
    line = NULL;
    free(toFree);
  }
  printf("tokens: %lld\n", tokens);
  fclose(ifp);
}

void solve_sim_equation(equation xTerms, equation yTerms, int64_t *pushes)
{
  int isSolvable;
  int64_t a, b, aFactor, simEquals;

  isSolvable = 1;
  aFactor = (xTerms.b * yTerms.a) - (yTerms.b * xTerms.a);
  simEquals = (yTerms.equals * xTerms.b) - (yTerms.b * xTerms.equals);

  if (simEquals % aFactor == 0) {
    a = simEquals / aFactor;
    if (((xTerms.equals - (xTerms.a * a)) % xTerms.b) == 0) {
      b = (xTerms.equals - (xTerms.a * a)) / xTerms.b;
      printf("a = %lld, b = %lld\n", a, b);
      *pushes = a;
      *(pushes + 1) = b;
    }
    else
      isSolvable = 0;
  }
  else
    isSolvable = 0;

  if (!isSolvable)
    printf("Can't solve:\n%llds + %lldt = %lld\n%llds + %lldt = %lld\n", xTerms.a, xTerms.b, xTerms.equals, yTerms.a, yTerms.b, yTerms.equals);
}
