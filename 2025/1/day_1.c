#include "day_1.h"

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
  input in;
  char *r;
  int i, sign, dial, move, pwd;

  in = alloc_input();

  dial = 50;
  pwd = 0;
  for (i = 0; i < in.len; i++) {
    r = *(in.rot + i);
    sign = *r++ == LEFT ? -1 : 1;
    move = sign * atoi(r);
    dial = (dial + (move % NUMS)) % NUMS;

    if (dial == 0)
      pwd++;
    else if (dial < 0)
      dial += NUMS;
  }

  printf("pwd: %d\n", pwd);
  free_input(in);
}

void part_2(void)
{
  input in;
  char *r;
  int i, sign, cur, prev, move, pwd;

  in = alloc_input();

  cur = 50;
  pwd = 0;
  for (i = 0; i < in.len; i++) {
    r = *(in.rot + i);
    sign = *r++ == LEFT ? -1 : 1;
    move = atoi(r);
    pwd += (move / NUMS);

    prev = cur;
    cur = (cur + ((sign * move) % NUMS)) % NUMS;

    if (cur == 0)
      pwd++;
    else if (cur < 0) {
      cur += NUMS;
      if (prev != 0)
        pwd++;
    }
    else if (prev > cur && sign == 1)
      pwd++;
  }

  printf("pwd: %d\n", pwd);
  free_input(in);
}


input alloc_input(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line;
  input retValue;

  ifp = fopen("input.txt", "r");
  linecap = 0;
  line = NULL;
  retValue.rot = NULL;
  retValue.len = 0;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    retValue.rot = realloc(retValue.rot, ++retValue.len * sizeof(char *));
    *(retValue.rot + retValue.len - 1) = line;
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

void free_input(input in)
{
  int i;

  for (i = 0; i < in.len; i++)
    free(*(in.rot + i));
  free(in.rot);
}
