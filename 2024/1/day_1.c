#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REALLOC_SIZE(i)          ((i / 2) + 1) * sizeof(int)
#define LOCATION_INDEX(loc, i)   *(*(loc) + (i / 2))

int read_locations(int **, int **);
int cmp(const void *, const void *);
int calculate_diff(const int *, const int *, int);
int calculate_sim(const int *, const int *, int);

int main(void)
{
  int **left, **right, answer, n;

  left = malloc(sizeof(int *));
  *left = malloc(sizeof(int));
  right = malloc(sizeof(int *));
  *right = malloc(sizeof(int));
  
  n = read_locations(left, right);

  qsort(*left, n, sizeof(int), cmp);
  qsort(*right, n, sizeof(int), cmp);
  
  answer = calculate_diff(*left, *right, n);
  printf("diff: %d\n", answer);

  answer = calculate_sim(*left, *right, n);
  printf("sim: %d\n", answer);
  
  free(*left);
  free(left);
  free(*right);
  free(right);

  return EXIT_SUCCESS;
}

int read_locations(int **left, int **right)
{
  char    *line, *token;
  FILE    *ifp;
  size_t  linecap;
  ssize_t linelen;
  int     i;

  line = NULL;
  linecap = i = 0;
  ifp = fopen("./input.txt", "r");
  
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    while ((token = strsep(&line, "  ")) != NULL)  {
      if (*token != '\0') {
        if (i % 2 == 0) {
          *left = realloc(*left, REALLOC_SIZE(i));
          LOCATION_INDEX(left, i) = atoi(token);
        }
        else {
          *right = realloc(*right, REALLOC_SIZE(i));
          LOCATION_INDEX(right, i)= atoi(token);
        }
        i++;
      }
    }
  }

  fclose(ifp);
  return i/2;
}

int cmp(const void *va, const void *vb)
{
  const int *a = va;
  const int *b = vb;
  int  diff = *a - *b;

  return ((diff >= 0) ? ((diff > 0) ? 1 : 0) : -1);
}

int calculate_diff(const int *left, const int *right, int n)
{
  int i, retValue;
  i = retValue = 0;

  while (i < n) {
    retValue += abs(*(left + i) - *(right + i));
    i++;
  }

  return retValue;
}

int calculate_sim(const int *left, const int *right, int n)
{
  int i, j, retValue;
  i = j = retValue = 0;

  for(; i < n; i++) {
    for(j = 0; j < n && *(right + j) <= *(left + i); j++) {
      if (*(left + i) == *(right + j))
        retValue += *(left + i);
    }
  }

  return retValue;
}
