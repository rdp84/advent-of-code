#include "day_2.h"


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
  char *nstr, *left, *right;
  int i, len;
  long total;

  in = alloc_input();
  nstr = malloc(NSTR_SIZE * sizeof(char));
  total = 0;

  for (i = 0; i < in.len; i++) {
    while (*(*(in.ranges + i)) <= *(*(in.ranges + i) + 1)) {
      if ((len = snprintf(NULL, 0, "%ld", *(*(in.ranges + i)))) % 2 == 0) {
        if (len >= NSTR_SIZE)
          nstr = realloc(nstr, (len + 1) * sizeof(char));
        snprintf(nstr, len + 1, "%ld", *(*(in.ranges + i)));

        left = nstr;
        right = nstr + (len / 2);
        while (*right && *left == *right) {
          left++;
          right++;
        }

        if (*right == '\0')
          total += *(*(in.ranges + i));
      }
      (*(*(in.ranges + i)))++;
    }
  }

  free(nstr);
  free_input(in);
  printf("total: %ld\n", total);
}

void part_2(void)
{
  input in;
  char *nstr, *left, *right;
  int i, len, winlen;
  long total;

  in = alloc_input();
  nstr = malloc(NSTR_SIZE * sizeof(char));
  total = 0;

  for (i = 0; i < in.len; i++) {
    while (*(*(in.ranges + i)) <= *(*(in.ranges + i) + 1)) {
      if ((len = snprintf(NULL, 0, "%ld", *(*(in.ranges + i)))) >= NSTR_SIZE)
        nstr = realloc(nstr, (len + 1) * sizeof(char));
      snprintf(nstr, len + 1, "%ld", *(*(in.ranges + i)));

      for (winlen = 1; winlen <= len / 2; winlen++) {
        if (len % winlen == 0) {
          left = nstr;
          right = nstr + winlen;
          while (*right && *left == *right) {
              left++;
              right++;
          }

          if (*right == '\0') {
            total += *(*(in.ranges + i));
            break;
          }
        }
      }
      (*(*(in.ranges + i)))++;
    }
  }

  free(nstr);
  free_input(in);
  printf("total: %ld\n", total);
}

input alloc_input(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *range, *id, *tofree;
  int i;
  input retValue;

  ifp = fopen("input.txt", "r");
  linecap = 0;
  line = NULL;
  retValue.len = 1;
  retValue.ranges = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    tofree = line;
    while (*line) {
      if (*line == ',')
        retValue.len++;
      line++;
    }
    retValue.ranges = malloc(retValue.len * sizeof(long *));
    for (i = 0; i < retValue.len; i++)
      *(retValue.ranges + i) = malloc(2 * sizeof(long));

    i = 0;
    line = tofree;
    while ((range = strsep(&line, ",")) != NULL) {
      id = strsep(&range, "-");
      *(*(retValue.ranges + i)) = atol(id);
      *(*(retValue.ranges + i) + 1) = atol(range);
      i++;
    }
    free(tofree);
  }

  free(line);
  fclose(ifp);
  return retValue;
}

void free_input(input in)
{
  int i;

  for (i = 0; i < in.len; i++)
    free(*(in.ranges + i));
  free(in.ranges);
}

void print_ranges(input in)
{
  int i;

  for (i = 0; i < in.len; i++)
    printf("%ld-%ld\n", *(*(in.ranges + i)), *(*(in.ranges + i) + 1));
}
