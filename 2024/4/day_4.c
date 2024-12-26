#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "day_4.h"

char **alloc_wordsearch(int *, int *);
void free_wordsearch(char **, int);

void part_1(void);
void part_2(void);

int main(void)
{
  part_1();
  part_2();
  return EXIT_SUCCESS;
}

char **alloc_wordsearch(int *rows, int *cols)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  *rows = *cols = 0;

  retValue = malloc(sizeof(char *));
  *cols = getline(&line, &linecap, ifp) - 1;
  *(retValue) = line;
  (*rows)++;
  line = NULL;

  while((linelen = getline(&line, &linecap, ifp)) > 0) {
    retValue = realloc(retValue, (*rows + 1) * sizeof(char *));
    *(retValue + *rows) = line;
    (*rows)++;
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

void free_wordsearch(char **wordSearch, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    free(*(wordSearch + i));
  free(wordSearch);
}

void part_1(void)
{
  char **wordSearch;
  int i, j, rows, cols, appearances;

  rows = cols = appearances = 0;
  wordSearch = alloc_wordsearch(&rows, &cols);

  for (i = 0; i < WORD_LENGTH - 1; i++) {
    for (j = 0; j < WORD_LENGTH - 1; j++) {
      if (check_x(i, j, wordSearch)) {
        check_east(i, j, wordSearch, appearances);
        check_south_east(i, j, wordSearch, appearances);
        check_south(i, j, wordSearch, appearances);
      }
    }

    for (j = WORD_LENGTH - 1; j < cols - 3; j++) {
      if (check_x(i, j, wordSearch)) {
        check_east(i, j, wordSearch, appearances);
        check_south_east(i, j, wordSearch, appearances);
        check_south(i, j, wordSearch, appearances);
        check_south_west(i, j, wordSearch, appearances);
        check_west(i, j, wordSearch, appearances);
      }
    }

    for (j = cols - 3; j < cols; j++) {
      if (check_x(i, j, wordSearch)) {
        check_south(i, j, wordSearch, appearances);
        check_south_west(i, j, wordSearch, appearances);
        check_west(i, j, wordSearch, appearances);
      }
    }
  }

  for (i = WORD_LENGTH - 1; i < rows - 3; i++) {
    for (j = 0; j < WORD_LENGTH - 1; j++) {
      if (check_x(i, j, wordSearch)) {
          check_north(i, j, wordSearch, appearances);
          check_north_east(i, j, wordSearch, appearances);
          check_east(i, j, wordSearch, appearances);
          check_south_east(i, j, wordSearch, appearances);
          check_south(i, j, wordSearch, appearances);
      }
    }

    for (j = WORD_LENGTH - 1; j < cols - 3; j++) {
      if (check_x(i, j, wordSearch)) {
          check_north(i, j, wordSearch, appearances);
          check_north_east(i, j, wordSearch, appearances);
          check_east(i, j, wordSearch, appearances);
          check_south_east(i, j, wordSearch, appearances);
          check_south(i, j, wordSearch, appearances);
          check_south_west(i, j, wordSearch, appearances);
          check_west(i, j, wordSearch, appearances);
          check_north_west(i, j, wordSearch, appearances);
      }
    }

    for (j = cols - 3; j < cols; j++) {
      if (check_x(i, j, wordSearch)) {
        check_north(i, j, wordSearch, appearances);
        check_south(i, j, wordSearch, appearances);
        check_south_west(i, j, wordSearch, appearances);
        check_west(i, j, wordSearch, appearances);
        check_north_west(i, j, wordSearch, appearances);
      }
    }
  }

  for (i = rows - 3; i < rows; i++) {
    for (j = 0; j < WORD_LENGTH - 1; j++) {
      if (check_x(i, j, wordSearch)) {
          check_north(i, j, wordSearch, appearances);
          check_north_east(i, j, wordSearch, appearances);
          check_east(i, j, wordSearch, appearances);
      }
    }

    for (j = WORD_LENGTH - 1; j < cols - 3; j++) {
      if (check_x(i, j, wordSearch)) {
        check_north(i, j, wordSearch, appearances);
        check_north_east(i, j, wordSearch, appearances);
        check_east(i, j, wordSearch, appearances);
        check_west(i, j, wordSearch, appearances);
        check_north_west(i, j, wordSearch, appearances);
      }
    }

    for (j = cols - 3; j < cols; j++) {
      if (check_x(i, j, wordSearch)) {
        check_north(i, j, wordSearch, appearances);
        check_west(i, j, wordSearch, appearances);
        check_north_west(i, j, wordSearch, appearances);
      }
    }
  }
  free_wordsearch(wordSearch, rows);
  printf("Number of xmas words: %d\n", appearances);
}

void part_2(void)
{
  char **wordSearch;
  int i, j, rows, cols, appearances;
  bool isNWtoSE, isNEtoSW;

  rows = cols = appearances = 0;
  wordSearch = alloc_wordsearch(&rows, &cols);

  for (i = 1; i < rows - 1; i++) {
    for (j = 1; j < cols - 1; j++) {
      isNWtoSE = isNEtoSW = false;
      
      if (check_a(i, j, wordSearch)) {
        if (check_m(i - 1, j - 1, wordSearch)) {
          if (check_s(i + 1, j + 1, wordSearch))
            isNWtoSE = true;
        }
        else if (check_s(i - 1, j - 1, wordSearch)) {
          if (check_m(i + 1, j + 1, wordSearch))
            isNWtoSE = true;
        }

        if (isNWtoSE) {
          if (check_m(i - 1, j + 1, wordSearch)) {
            if (check_s(i + 1, j - 1, wordSearch))
              isNEtoSW = true;
          }
          else if(check_s(i - 1, j + 1, wordSearch)) {
            if (check_m(i + 1, j - 1, wordSearch))
              isNEtoSW = true;
          }

          if (isNEtoSW)
            appearances++;
        }
      }
    }
  }
  printf("Number of x-mas: %d\n", appearances);
  free_wordsearch(wordSearch, rows);
}
