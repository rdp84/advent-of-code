#include "day_3.h"

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
  long  total;

  total = total_joltage(2);
  printf("total: %ld\n", total);
}

void part_2(void)
{
  long  total;

  total = total_joltage(12);
  printf("total: %ld\n", total);
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
  retValue.len = 0;
  retValue.banks = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 0)
    retValue.len++;
  retValue.banks = malloc(retValue.len * sizeof(char *));

  rewind(ifp);
  free(line);
  line = NULL;

  i = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    *(line + linelen - 1) = '\0';
    *(retValue.banks + i++) = line;
    line = NULL;
  }

  free(line);
  fclose(ifp);
  return retValue;
}

void free_banks(input in)
{
  int i;

  for (i = 0; i < in.len; i++)
    free(*(in.banks + i));
  free(in.banks);
}

void print_banks(input in)
{
  int i;

  for (i = 0; i < in.len; i++)
    printf("%s\n", *(in.banks + i));
}

long total_joltage(int batlen)
{
  char *bank, *batteries, c;
  int i, j, k, banklen, start, end, cindex;
  input in;
  long retValue;

  in = alloc_input();
  retValue = 0;
  batteries = malloc((batlen + 1) * sizeof(char));
  *(batteries + batlen) = '\0';

  for (i = 0; i < in.len; i++) {
    bank = *(in.banks + i);
    banklen = strlen(bank);

    start = 0;
    for (j = 0; j < batlen; j++) {
      end = banklen - batlen + j;
      c = '\0';

      for (k = start; k <= end; k++) {
        if (*(bank + k) > c) {
          c = *(bank + k);
          cindex = k;
        }
      }
      *(batteries + j) = c;
      start = cindex + 1;
    }
    retValue += atol(batteries);
  }

  free(batteries);
  free_banks(in);
  return retValue;
}
