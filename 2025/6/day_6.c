#include "day_6.h"

void part_1(void);
void part_2(void);

int main(void)
{
  part_1();
  part_2();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  long cnt;

  cnt = solve(&solve_row);
  printf("cnt: %ld\n", cnt);
}

void part_2(void)
{
  long cnt;

  cnt = solve(&solve_col);
  printf("cnt: %ld\n", cnt);
}

input alloc_input(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line;
  int i;
  input retValue;

  ifp = fopen("input.txt", "r");
  retValue.rows = 0;
  linecap = 0;
  line = NULL;
  while (getline(&line, &linecap, ifp) > 0)
    retValue.rows++;
  free(line);
  rewind(ifp);
  retValue.probs = malloc(retValue.rows * sizeof(char *));

  line = NULL;
  i = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    *(line + linelen - 1) = '\0';
    *(retValue.probs + i++) = line;
    line = NULL;
  }
  free(line);
  fclose(ifp);

  return retValue;
}

void free_input(input in)
{
  int i;

  for (i = 0; i < in.rows; i++)
    free(*(in.probs + i));
  free(in.probs);
}

int find_sep(input in, int rpos)
{
  bool issep;
  int i, j;

  for (j = rpos; j >= 0; j--) {
    issep = true;
    for (i = 0; i < in.rows && issep; i++)
      issep = *(*(in.probs + i) + j) == SPACE;
    if (issep)
      return j;
  }
  return j;
}

long solve(long (*fct) (input, int, int))
{
  input in;
  int lpos, rpos;
  long retValue;

  in = alloc_input();

  rpos = strlen(*(in.probs)) - 1;
  retValue = 0;
  while(rpos > 0) {
    lpos = find_sep(in, rpos);
    retValue += fct(in, lpos, rpos);
    rpos = lpos - 1;
  }
  free_input(in);

  return retValue;
}

long solve_row(input in, int lpos, int rpos)
{
  char *nstr, op;
  int i, j, nstrlen, numslen, *rnums;
  long retValue;

  nstrlen = rpos - lpos;
  nstr = malloc((nstrlen + 1) * sizeof(char));
  *(nstr + nstrlen) = '\0';

  numslen = in.rows - 1;
  rnums = malloc(numslen * sizeof(int));

  for (i = 0; i < numslen; i++) {
    for (j = lpos + 1; j <= rpos; j++)
      *nstr++ = *(*(in.probs + i) + j);
    nstr -= nstrlen;
    *rnums++ = atoi(nstr);
  }
  op = *(*(in.probs + numslen) + lpos + 1);

  rnums -= numslen;
  retValue = op == ADD ? add(rnums, numslen) : mul(rnums, numslen);
  free(nstr);
  free(rnums);

  return retValue;
}

long solve_col(input in, int lpos, int rpos)
{
  char *nstr, op;
  int i, j, numslen, *cnums;
  long retValue;

  nstr = malloc(in.rows * sizeof(char));
  *(nstr + in.rows - 1) = '\0';

  numslen = rpos - lpos;
  cnums = malloc(numslen * sizeof(int));

  for (j = rpos; j > lpos; j--) {
    for (i = 0; i < in.rows - 1; i++)
      *(nstr + i) = *(*(in.probs + i) + j);
    *cnums++ = atoi(nstr);
  }
  op = *(*(in.probs + in.rows - 1) + lpos + 1);

  cnums -= numslen;
  retValue = op == ADD ? add(cnums, numslen) : mul(cnums, numslen);
  free(nstr);
  free(cnums);

  return retValue;
}

long add(int *nums, int len)
{
  int i;
  long retValue;

  retValue = 0;
  for (i = 0; i < len; i++)
    retValue += *(nums + i);
  return retValue;
}

long mul(int *nums, int len)
{
  int i;
  long retValue;

  retValue = 1;
  for (i = 0; i < len; i++)
    retValue *= *(nums + i);
  return retValue;
}
