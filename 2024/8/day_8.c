#include "day_8.h"

void part_1(void);
void part_2(void);

int main(void)
{
  part_1();
  part_2();

  return EXIT_SUCCESS;
}

void part_1(void)
{
  antenna *antennas;
  int xBound, yBound;

  xBound = yBound = 0;
  antennas = alloc_antennas(&xBound, &yBound);
  count_unique_antinodes_part_1(antennas, xBound, yBound);
  free_antennas(antennas);
}

void part_2(void)
{
  antenna *antennas;
  int xBound, yBound;

  xBound = yBound = 0;
  antennas = alloc_antennas(&xBound, &yBound);
  count_unique_antinodes_part_2(antennas, xBound, yBound);
  free_antennas(antennas);
}

antenna *alloc_antennas(int *xBound, int *yBound)
{
  FILE *ifp;
  char *line, *toFree, c;
  size_t linecap;
  ssize_t linelen;
  antenna *retValue, *tail;
  coord *coords;
  int x, y;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  retValue = NULL;
  y = 0;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    toFree = line;

    x = 0;
    while ((c = *line++)) {
      if (c == '.') {
        x++;
      }
      else if (c == '\n')
        continue;
      else {
        if (retValue == NULL) {
          retValue = alloc_antenna(c, x, y);
        }
        else {
          tail = retValue;
          while (tail != NULL) {
            if (tail->c == c) {
              coords = tail->coords;
              while (coords->next != NULL)
                coords = coords->next;
              coords->next = alloc_coord(x, y);
              tail = NULL;
            }
            else if (tail->next == NULL) {
              tail->next = alloc_antenna(c, x, y);
              tail = NULL;
            }
            else
              tail = tail->next;
          }
        }
        x++;
      }
    }
    y--;
    line = NULL;
    free(toFree);
  }
  fclose(ifp);
  *xBound = x;
  *yBound = y;
  return retValue;
}

antenna *alloc_antenna(char c, int x, int y)
{
  antenna *retValue;

  retValue = malloc(sizeof(antenna));
  retValue->c = c;
  retValue->coords = alloc_coord(x, y);
  retValue->next = NULL;

  return retValue;
}

coord *alloc_coord(int x, int y)
{
  coord *retValue;

  retValue = malloc(sizeof(coord));
  retValue->x = x;
  retValue->y = y;
  retValue->next = NULL;

  return retValue;
}

char **alloc_antinodes(int xBound, int yBound)
{
  char **retValue;
  int i, j;

  retValue = malloc(-yBound * sizeof(char *));
  for (i = 0; i < -yBound; i++) {
    *(retValue + i) = malloc((xBound + 1) * sizeof(char));
    for (j = 0; j < xBound; j++) 
      *(*(retValue + i) + j) = '.';
    *(*(retValue + i) + j) = '\0';
  }

  return retValue;
}

void free_antennas(antenna *antennas)
{
  antenna *toFreeAnt;
  coord *toFreeCoords;

  while (antennas != NULL) {
    while (antennas->coords != NULL) {
      toFreeCoords = antennas->coords;
      antennas->coords = antennas->coords->next;
      free(toFreeCoords);
    }
    toFreeAnt = antennas;
    antennas = antennas->next;
    free(toFreeAnt);
  }
}

int free_antinodes(char **antinodes, int xBound, int yBound)
{
  int i, j, retValue;

  retValue = 0;
  for (i = 0; i < -yBound; i++) {
    for (j = 0; j < xBound; j++)
      if (is_antinode(antinodes, i, j))
        retValue++;
    free(*(antinodes + i));
  }

  return retValue;
}

void print_antennas(antenna *antennas)
{
  coord *coords;
  while (antennas != NULL) {
    printf("antenna: %c has coords ", antennas->c);
    coords = antennas->coords;
    while (coords != NULL) {
      printf("(%d, %d) ", coords->x, coords->y);
      coords = coords->next;
    }
    antennas = antennas->next;
    printf("\n");
  }
}

coord vector_a_b(coord *a, coord *b)
{
  coord retValue;

  retValue.x = b->x - a->x;
  retValue.y = b->y - a->y;
  retValue.next = NULL;

  return retValue;
}

void count_unique_antinodes_part_1(antenna *antennas, int xBound, int yBound)
{
  coord *hCoord, *tCoord, vectorAB, vectorBA;
  int x, y;
  char **antinodes;

  antinodes = alloc_antinodes(xBound, yBound);

  while (antennas != NULL) {
    hCoord = antennas->coords;

    while (hCoord != NULL) {
      tCoord = hCoord->next;
      while (tCoord != NULL) {
        vectorAB = vector_a_b(hCoord, tCoord);
        x = hCoord->x + (2 * vectorAB.x);
        y = hCoord->y + (2 * vectorAB.y);
        if (inside_x_bound(x, xBound) && inside_y_bound(y, yBound))
          add_antinode(antinodes, x, y);

        vectorBA = vector_a_b(tCoord, hCoord);
        x = tCoord->x + (2 * vectorBA.x);
        y = tCoord->y + (2 * vectorBA.y);
        if (inside_x_bound(x, xBound) && inside_y_bound(y, yBound))
          add_antinode(antinodes, x, y);

        tCoord = tCoord->next;
      }
      hCoord = hCoord->next;
    }
    antennas = antennas->next;
  }
  printf("Number of unique antinodes %d\n", free_antinodes(antinodes, xBound, yBound));
}


void count_unique_antinodes_part_2(antenna *antennas, int xBound, int yBound)
{
  coord *hCoord, *tCoord, vectorAB, vectorBA;
  int x, y;
  char **antinodes;

  antinodes = alloc_antinodes(xBound, yBound);

  while (antennas != NULL) {
    hCoord = antennas->coords;

    while (hCoord != NULL) {
      tCoord = hCoord->next;
      while (tCoord != NULL) {
        vectorAB = vector_a_b(hCoord, tCoord);
        x = hCoord->x + (vectorAB.x);
        y = hCoord->y + (vectorAB.y);
        add_all_inline_antinodes(antinodes, x, xBound, y, yBound, vectorAB)

        vectorBA = vector_a_b(tCoord, hCoord);
        x = tCoord->x + (vectorBA.x);
        y = tCoord->y + (vectorBA.y);
        add_all_inline_antinodes(antinodes, x, xBound, y, yBound, vectorBA);

        tCoord = tCoord->next;
      }
      hCoord = hCoord->next;
    }
    antennas = antennas->next;
  }
  printf("Number of unique antinodes %d\n", free_antinodes(antinodes, xBound, yBound));
}
