#include "day_7.h"

void part_1(void);
void part_2(void);

int main(void)
{
  part_1();
  /* part_2(); */

  return EXIT_SUCCESS;
}

void part_1(void)
{
  input in;
  int cnt;

  in = alloc_input();

  *(*(in.diag) + in.sindex) = EMPTY;
  cnt = 0;
  send_beam(in, 0, in.sindex, &cnt);
  printf("cnt: %d\n", cnt);

  free_input(in);
}

void part_2(void)
{
  input in;
  splitter **s;
  long cnt;

  in = alloc_input();
  s = alloc_splitters(in);

  *(*(in.diag) + in.sindex) = EMPTY;
  cnt = timelines(in, 0, in.sindex, s);
  printf("cnt: %ld\n", cnt);

  free_splitters(s, in.rows);
  free_input(in);
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

  linelen = getline(&line, &linecap, ifp);
  i = 0;
  while (*(line + i++) != START) ;
  retValue.sindex = --i;
  retValue.cols = linelen - 1;
  retValue.rows = 1;
  while (getline(&line, &linecap, ifp) > 1)
    retValue.rows++;
  retValue.diag = malloc(retValue.rows * sizeof(char *));
  free(line);
  line = NULL;
  rewind(ifp);

  i = 0;
  while (getline(&line, &linecap, ifp) > 0) {
    *(retValue.diag + i++) = line;
    line = NULL;
  }
  free(line);
  fclose(ifp);

  return retValue;
}

void print_input(input in)
{
  int i;

  for (i = 0; i < in.rows; i++)
    printf("%s", *(in.diag + i));
  printf("sindex: %d\n", in.sindex);
}

void free_input(input in)
{
  while (--in.rows >= 0)
    free(*(in.diag + in.rows));
  free(in.diag);
  in.diag = NULL;
}

splitter **alloc_splitters(input in)
{
  int i, j;
  splitter **retValue;

  retValue = malloc(in.rows * sizeof(splitter *));
  for (i = 0; i < in.rows; i++) {
    *(retValue + i) = malloc(in.cols * sizeof(splitter));
    for (j = 0; j < in.cols; j++) {
      (*(*(retValue + i) + j)).visited = false;
      (*(*(retValue + i) + j)).timelines = 0;
    }
  }

  return retValue;
}

void free_splitters(splitter **s, int len)
{
  while (--len >= 0)
    free(*(s + len));
  free(s);
}

void send_beam(input in, int i, int j, int *cnt)
{
  bpath p;

  p = beam_path(in, i, j);
  if (p.splits) {
    *cnt += 1;
    if (p.left)
      send_beam(in, p.i, p.j - 1, cnt);
    if (p.right)
      send_beam(in, p.i, p.j + 1, cnt);
  }
}

bpath beam_path(input in, int i, int j)
{
  char c, **diag;
  bpath retValue;

  diag = in.diag;
  retValue.splits = retValue.left = retValue.right = false;
  retValue.i = retValue.j = -1;

  *(*(diag + i++) + j) = BEAM;
  while (i < in.rows && (c = *(*(diag + i++) + j)) == EMPTY) ;

  if (c == SPLITTER) {
    retValue.i = --i;
    retValue.j = j;
    retValue.left = *(*(diag + i) + j - 1) == EMPTY;
    retValue.right = *(*(diag + i) + j + 1) == EMPTY;
    retValue.splits = retValue.left || retValue.right;
  }

  return retValue;
}

long timelines(input in, int i, int j, splitter **spls)
{
  qpath p;
  splitter *s;

  p = quantum_path(in, i, j);
  if (p.splits) {
    s = &(*(*(spls + p.i) + p.j));
    if (!s->visited) {
      s->timelines = timelines(in, p.i, p.j - 1, spls) + timelines(in, p.i, p.j + 1, spls);
      s->visited = true;
    }
    return s->timelines;
  }
  else
    return 1;
}

qpath quantum_path(input in, int i, int j)
{
  char c, **diag;
  qpath retValue;

  diag = in.diag;
  retValue.splits = false;
  retValue.i = retValue.j = -1;

  while (i < in.rows && (c = *(*(diag + i++) + j)) == EMPTY) ;

  if (c == SPLITTER) {
    retValue.splits = true;
    retValue.i = --i;
    retValue.j = j;
  }

  return retValue;
}
