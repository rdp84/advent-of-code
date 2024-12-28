#include "day_6.h"

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
  char **map;
  int rows, cols;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  walk_map(map, rows, cols);
  free_map(map, rows);
}

void part_2(void)
{
  char **map;
  int rows, cols;

  rows = cols = 0;
  map = alloc_map(&rows, &cols);
  find_loops(map, rows, cols);
  free_map(map, rows);
}

char **alloc_map(int *rows, int *cols)
{
  FILE *ifp;
  char *line, **retValue;
  size_t linecap;
  ssize_t linelen;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
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

  free(line);
  fclose(ifp);
  return retValue;
}

void find_start(char **map, int rows, int cols, char *startDir, int *startRow, int *startCol)
{
  char dir;
  int i, j;
  bool foundStartPos;

  for (i = 0, foundStartPos = false; i < rows && !foundStartPos; i++) {
    for (j = 0; j < cols && !foundStartPos; j++) {
      dir = *(*(map + i) + j);
      foundStartPos = dir == NORTH || dir == EAST || dir == SOUTH || dir == WEST;
    }
  }

  *startDir = dir;
  *startRow = i - 1;
  *startCol = j - 1;  
}

void walk_map(char **map, int rows, int cols)
{
  char dir;
  int i, j, uniqueVisits;

  find_start(map, rows, cols, &dir, &i, &j);

  uniqueVisits = 0;
  while (i >= 0 && i < rows &&  j >= 0 && j < cols) {
    if (dir == NORTH) {
      if (i == 0 || !obstacle_north(i, j, map)) {
        mark_visited(i, j, map, uniqueVisits);
        i--;
      }
      else
        dir = EAST;
    }
    else if (dir == EAST) {
      if (j == cols - 1 || !obstacle_east(i, j, map)) {
        mark_visited(i, j, map, uniqueVisits);
        j++;
      }
      else
        dir = SOUTH;
    }
    else if (dir == SOUTH) {
      if (i == rows - 1 || !obstacle_south(i, j, map)) {
        mark_visited(i, j, map, uniqueVisits);
        i++;
      }
      else
        dir = WEST;
    }
    else {
      if (j == 0 || !obstacle_west(i, j, map)) {
        mark_visited(i, j, map, uniqueVisits);
        j--;
      }
      else
        dir = NORTH;
    }
  }
  printf("number of unique visits: %d\n", uniqueVisits);
}

void find_loops(char **map, int rows, int cols)
{
  char dir, startDir;
  int loops, i, j, startRow, startCol, ii, jj;
  visited_locations *vl;
  bool foundLoop;

  vl = alloc_visited_locations();
  find_start(map, rows, cols, &startDir, &startRow, &startCol);
  loops = 0;
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {

      if ((i == startRow && j == startCol) ||
          obstacle_current(i, j, map)) {
        continue;
      }
      else {
        add_obstacle(i, j, map);
        dir = startDir;
        ii = startRow;
        jj = startCol;
        foundLoop = false;

        while (ii >= 0 && ii < rows &&  jj >= 0 && jj < cols && !foundLoop) {
          add_visited_location(vl, ii, jj, dir);
          if (dir == NORTH) {
            if (ii == 0 || !obstacle_north(ii, jj, map))
              ii--;
            else
              dir = EAST;
          }
          else if (dir == EAST) {
            if (jj == cols - 1 || !obstacle_east(ii, jj, map))
              jj++;
            else
              dir = SOUTH;
          }
          else if (dir == SOUTH) {
            if (ii == rows - 1 || !obstacle_south(ii, jj, map))
              ii++;
            else
              dir = WEST;
          }
          else {
            if (jj == 0 || !obstacle_west(ii, jj, map))
              jj--;
            else
              dir = NORTH;
          }
          foundLoop = check_for_loop(vl, ii, jj, dir);
        }
        if (foundLoop) {
          loops++;
          printf("found a loop by adding obstacle at row %d, col %d\n", i, j);
        }
        else {
          printf("no loop found by adding obstacle at row %d, col %d\n", i, j);
        }
        set_free(i, j, map);
        reset_visited_locations(vl);
      }
    }
  }
  free_visited_locations(vl);
  printf("found loops: %d\n", loops);
}

void free_map(char **map, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    free(*(map + i));
  free(map);
}

void print_map(char **map, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    printf("%s", *(map + i));
}

visited_locations *alloc_visited_locations(void)
{
  visited_locations *retValue;

  retValue = malloc(sizeof(visited_locations));
  retValue->visits = 0;
  retValue->locations = NULL;

  return retValue;
}

void add_visited_location(visited_locations *vl, int row, int col, char dir)
{
  location *l, *head;

  l = malloc(sizeof(location));
  l->row = row;
  l->col = col;
  l->dir = dir;
  l->next = NULL;

  if (vl->locations == NULL) {
    vl->locations = l;
  }
  else {
    head = vl->locations;
    while (head->next != NULL)
      head = head->next;
    head->next = l;
  }
  vl->visits++;
}

bool check_for_loop(visited_locations *vl, int row, int col, char dir)
{
  location *head;
  bool retValue;

  retValue = false;
  head = vl->locations;
  while (head != NULL && !retValue) {
    retValue = head->row == row && head->col == col && head->dir == dir;
    head = head->next;
  }

  return retValue;
}

void reset_visited_locations(visited_locations *vl)
{
  location *toFree;

  while (vl->locations != NULL) {
    toFree = vl->locations;
    vl->locations = vl->locations->next;
    free(toFree);
  }

  vl->visits = 0;
}

void print_visited_locations(visited_locations *vl)
{
  location *head;

  printf("number of locations visited: %d\n", vl->visits);

  head = vl->locations;
  while (head != NULL) {
    printf("(%d, %d, %c) ", head->row, head->col, head->dir);
    head = head->next;
  }
  printf("\n");
}

void free_visited_locations(visited_locations *vl)
{
  location *toFree;

  while (vl->locations != NULL) {
    toFree = vl->locations;
    vl->locations = vl->locations->next;
    free(toFree);
  }

  free(vl);
}
