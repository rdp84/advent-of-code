#include "day_4.h"

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
  int i, j, cnt;
  input in;

  in = alloc_input();
  print_grid(in);

  cnt = 0;
  for (i = 0; i < in.rlen; i++)
    for (j = 0; j < in.clen; j++)
      cnt += can_access(in, i, j) ? 1 : 0;
  free_grid(in);
  printf("cnt: %d\n", cnt);
}

void part_2(void)
{
  int i, j, cnt;
  input in;

  in = alloc_input();
  print_grid(in);

  cnt = 0;
  for (i = 0; i < in.rlen; i++)
    for (j = 0; j < in.clen; j++)
      cnt += remove_roll(in, i, j);

  free_grid(in);
  printf("cnt: %d\n", cnt);
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
  linecap = 0;
  line = NULL;
  retValue.rlen = retValue.clen = 0;
  retValue.grid = NULL;
  while (getline(&line, &linecap, ifp) > 1)
    retValue.rlen++;
  retValue.grid = malloc(retValue.rlen * sizeof(char *));

  rewind(ifp);
  free(line);
  line = NULL;
  i = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    *(line + linelen - 1) = '\0';
    *(retValue.grid + i++) = line;
    line = NULL;
    linecap = 0;
  }
  retValue.clen = strlen(*(retValue.grid));

  free(line);
  fclose(ifp);
  return retValue;
}

void free_grid(input in)
{
  int i;

  for (i = 0; i < in.rlen; i++)
    free(*(in.grid + i));
  free(in.grid);
}

void print_grid(input in)
{
  int i;

  for (i = 0; i < in.rlen; i++)
    printf("%s\n", *(in.grid + i));
}

bool can_access(input in, int i, int j)
{
  int cnt;

  if (!roll_at_index(in.grid, i, j))
    return false;

  cnt = 0;
  if (i == 0) {
    if (j == 0 || j == in.clen - 1)
      return true;
    else {
      cnt += roll_at_e(in.grid, i, j);
      cnt += roll_at_se(in.grid, i, j);
      cnt += roll_at_s(in.grid, i, j);
      cnt += roll_at_sw(in.grid, i, j);
      cnt += roll_at_w(in.grid, i, j);
    }
  } else if (i < in.rlen - 1) {
    cnt += roll_at_n(in.grid, i, j);
    cnt += roll_at_s(in.grid, i, j);
    if (j == 0) {
      cnt += roll_at_ne(in.grid, i, j);
      cnt += roll_at_e(in.grid, i, j);
      cnt += roll_at_se(in.grid, i, j);
    } else if (j < in.clen - 1) {
      cnt += roll_at_ne(in.grid, i, j);
      cnt += roll_at_e(in.grid, i, j);
      cnt += roll_at_se(in.grid, i, j);
      cnt += roll_at_sw(in.grid, i, j);
      cnt += roll_at_w(in.grid, i, j);
      cnt += roll_at_nw(in.grid, i, j);
    } else {
      cnt += roll_at_sw(in.grid, i, j);
      cnt += roll_at_w(in.grid, i, j);
      cnt += roll_at_nw(in.grid, i, j);
    }
  } else {
    if (j == 0 || j == in.clen - 1)
      return true;
    else {
      cnt += roll_at_n(in.grid, i, j);
      cnt += roll_at_ne(in.grid, i, j);
      cnt += roll_at_e(in.grid, i, j);
      cnt += roll_at_w(in.grid, i, j);
      cnt += roll_at_nw(in.grid, i, j);
    }
  }
  return cnt < 4;
}

int remove_roll(input in, int i, int j)
{
  if (i < 0 || i >= in.rlen ||
      j < 0 || j >= in.clen)
    return 0;
  else if (!can_access(in, i, j))
    return 0;
  else {
    *(*(in.grid + i) + j) = EMPTY;
    return 1 +
      remove_roll(in, i-1, j)   +
      remove_roll(in, i-1, j+1) +
      remove_roll(in, i, j+1)   +
      remove_roll(in, i+1, j+1) +
      remove_roll(in, i+1, j)   +
      remove_roll(in, i+1, j-1) +
      remove_roll(in, i, j-1)   +
      remove_roll(in, i-1, j-1);
  }
}
