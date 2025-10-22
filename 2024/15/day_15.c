#include "day_15.h"

int main(void)
{
  int mapRows, mapCols, movesRows, movesCols;
  char **map, **moves;

  mapRows = mapCols = movesRows = movesCols = 0;
  map = alloc_map(&mapRows, &mapCols);
  moves = alloc_moves(&movesRows, &movesCols);

  print_input(map, mapRows, mapCols);
  print_input(moves, movesRows, movesCols);

  free_input(map, mapRows);
  free_input(moves, movesRows);

  return EXIT_SUCCESS;
}

char **alloc_map(int *rows, int *cols)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("example_input.txt", "r");
  *rows = *cols = 0;

  retValue = malloc(sizeof(char *));
  *cols = getline(&line, &linecap, ifp) - 1;
  *(retValue) = line;
  (*rows)++;
  line = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    retValue = realloc(retValue, (*rows + 1) * sizeof(char *));
    *(retValue + *rows) = line;
    (*rows)++;
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

char **alloc_moves(int *rows, int *cols)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("example_input.txt", "r");

  while (getline(&line, &linecap, ifp) > 1) ;

  *rows = *cols = 0;
  retValue = malloc(sizeof(char *));
  *cols = getline(&line, &linecap, ifp) - 1;
  *(retValue) = line;
  (*rows)++;
  line = NULL;

  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    retValue = realloc(retValue, (*rows + 1) * sizeof(char *));
    *(retValue + *rows) = line;
    (*rows)++;
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

void print_input(char **input, int rows, int cols)
{
  int i, j;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++)
      printf("%c", *(*(input + i) + j));
    printf("\n");
  }
}

void free_input(char **map, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    free(*(map + i));
  free(map);
}
