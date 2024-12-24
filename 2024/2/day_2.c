#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int  read_report(char *, int **);
void parse_reports_part_1(void);
void parse_reports_part_2(void);

int main(void)
{
  parse_reports_part_1();
  parse_reports_part_2();

  return EXIT_SUCCESS;
}

int read_report(char *line, int **report)
{
  char *token;
  int levels;

  levels = 0;
  token = strsep(&line, " ");
  while (token != NULL) {
    *report = realloc(*report, (levels + 1) * sizeof(int));
    *(*(report) + levels) = atoi(token);
    token = strsep(&line, " ");
    levels++;
  }

  return levels;
}

void parse_reports_part_1(void)
{
  char *line;
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  int **report, levels, reports, i, safe, left, right, diff;
  bool isSafe, isIncreasing;

  report = malloc(sizeof(int *));
  *report = malloc(sizeof(int));
  line = NULL;
  linecap = reports = safe = 0;
  ifp = fopen("input.txt", "r");

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    reports++;
    levels = read_report(line, report);
    isSafe = true;

    for (i = 0; i < levels - 1 && isSafe; i++) {
      left = *(*(report) + i);
      right = *(*(report) + i + 1);

      if (i == 0) {
        isIncreasing = left < right ? true : false;
      }

      diff = isIncreasing ? right - left : left - right;
      isSafe = diff >= 1 && diff <= 3;
    }

    if (isSafe)
      safe++;
    
    free(line);
    line = NULL;
    linecap = 0;
  }

  printf("out of %d reports, %d were safe, %d were unsafe\n", reports, safe, reports - safe);
  free(*report);
  free(report);
  fclose(ifp);
}

void parse_reports_part_2(void)
{
  char *line;
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  int **report, levels, reports, i, j, limit, isIncreasing, safe, left, right, diff;
  bool isSafe;

  report = malloc(sizeof(int *));
  *report = malloc(sizeof(int));
  line = NULL;
  linecap = reports = safe = 0;
  ifp = fopen("input.txt", "r");

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    reports++;
    levels = read_report(line, report);

    for (i = 0; i < levels; i++) {
      isIncreasing = diff = 0;
      limit = i != levels - 1 ? levels - 1 : levels - 2;
      isSafe = true;

      for (j = 0; j < limit && isSafe;) {
        if (j == i) {
          left = *(*(report) + j + 1);
          right = *(*(report) + j + 2);
          j += 2;
        } else if ((j + 1) == i) {
          left = *(*(report) + j);
          right = *(*(report) + j + 2);
          j += 2;
        } else {
          left = *(*(report) + j);
          right = *(*(report) + j + 1);
          j++;
        }

        if (isIncreasing == 0)
          isIncreasing = left < right ? 1 : -1;

        diff = isIncreasing == 1 ? right - left : left - right;
        isSafe = diff >= 1 && diff <= 3;
      }

      if (isSafe) {
        safe++;
        break;
      }
    }
    free(line);
    line = NULL;
    linecap = 0;
  }
  printf("out of %d reports, %d were safe, %d were unsafe\n", reports, safe, reports - safe);
  free(*report);
  free(report);
  fclose(ifp);
}

