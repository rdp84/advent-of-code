#include "day_6.h"

void part_1(void);

int main(void)
{
  part_1();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  FILE *ifp;
  input in;
  int i, j, vlen, hlen;

  ifp = open_input();
  vlen = get_vertical_len(ifp);
  hlen = get_horizontal_len(ifp);
  printf("hlen: %d, vlen: %d\n", hlen, vlen);
  in = alloc_input(ifp, vlen, hlen);

  for (i = 0; i < in.vlen; i++) {
    for (j = 0; j < in.hlen; j++) {
      printf("%5d ", *(*(in.nums + i) + j));
    }
    printf("\n");
  }

  for (i = 0; i < in.vlen; i++)
    free(*(in.nums + i));
  free(in.nums);
  close_input(ifp);
}

FILE *open_input(void)
{
  return fopen("example_input.txt", "r");
}

void close_input(FILE *ifp)
{
  fclose(ifp);
}

int get_vertical_len(FILE *ifp)
{
  size_t linecap;
  char *line;
  int retValue;

  linecap = 0;
  line = NULL;
  retValue = 0;
  while (getline(&line, &linecap, ifp) > 0)
    retValue++;

  rewind(ifp);
  free(line);
  return retValue;
}

int get_horizontal_len(FILE *ifp)
{
  size_t linecap;
  char *line, *tofree;
  int numlen, retValue;

  linecap = 0;
  line = NULL;
  (void) getline(&line, &linecap, ifp);
  tofree = line;
  retValue = 0;
  while (*line && *line != '\n') {
    while (isspace(*line))
      line++;

    numlen = 0;
    while (isdigit(*line)) {
      numlen++;
      line++;
    }

    if (numlen > 0) {
      *line = '\0';
      printf("num: %d\n", atoi((line - numlen)));
      retValue++;
      line++;
    }
  }

  rewind(ifp);
  free(tofree);
  return retValue;
}

input alloc_input(FILE *ifp, int vlen, int hlen)
{
  size_t linecap;
  char *line, *tofree;
  int i, j, numlen;
  input retValue;

  retValue.vlen = vlen - 1;
  retValue.hlen = hlen;
  retValue.nums = malloc(retValue.vlen * sizeof(int *));
  for (i = 0; i < retValue.vlen; i++)
    *(retValue.nums + i) = malloc(retValue.hlen * sizeof(int));

  linecap = 0;
  line = NULL;
  i = 0;
  while (i < retValue.vlen) {
    (void) getline(&line, &linecap, ifp);
    tofree = line;
    j = 0;
    while (*line && *line != '\n') {
      while (isspace(*line))
        line++;

      numlen = 0;
      while (isdigit(*line)) {
        numlen++;
        line++;
      }

      if (numlen > 0) {
        *line = '\0';
        *(*(retValue.nums + i) + j) = atoi((line - numlen));
        line++;
        j++;
      }
    }
    i++;
    free(tofree);
    line = NULL;
  }

  rewind(ifp);
  return retValue;
}
