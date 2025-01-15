#include "day_12.h"

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
  char **map, plant;
  int rows, cols, i, j, *areaPerim, totalPrice;
  
  rows = cols = totalPrice = 0;
  areaPerim = malloc(2 * sizeof(int));
  map = alloc_map(&rows, &cols);

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      plant = *(*(map + i) + j);
      if (plant != '.') {
        *areaPerim = *(areaPerim + 1) = 0;
        walk_region(plant, map, rows, cols, i, j, areaPerim);
        totalPrice += *areaPerim * *(areaPerim + 1);
      }
    }
  }
  printf("totalPrice: %d\n", totalPrice);
  free(areaPerim);
  free_map(map, rows);
}

void part_2(void)
{
  char **map, plant;
  int rows, cols, i, j, totalPrice;
  region *r;
  
  rows = cols = totalPrice = 0;
  map = alloc_map(&rows, &cols);
  r = NULL;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      plant = *(*(map + i) + j);
      if (plant != '.') {
        r = alloc_region(plant);
        walk_region_2(plant, map, rows, cols, i, j, r);
        r->totalSides = total_sides(r->northSides) + total_sides(r->eastSides) + total_sides(r->southSides) + total_sides(r->westSides);
        totalPrice += r->area * r->totalSides;

        free_region(r);
      }

    }
  }
  printf("totalPrice: %d\n", totalPrice);
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

  retValue = malloc(sizeof(char *));
  *cols = getline(&line, &linecap, ifp) - 1;
  *retValue = line;

  line = NULL;
  *rows = 1;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    retValue = realloc(retValue, (*rows + 1) * sizeof(char *));
    *(retValue + *rows) = line;
    line = NULL;
    *rows += 1;
  }

  return retValue;
}

side *alloc_side(int index, int plant)
{
  side *retValue;

  retValue = malloc(sizeof(side));
  retValue->index = index;
  retValue->size = 1;
  retValue->plants = malloc(sizeof(int));
  *(retValue->plants) = plant;
  retValue->next = NULL;

  return retValue;
}

region *alloc_region(char plant)
{
  region *retValue;

  retValue = malloc(sizeof(region));
  retValue->plant = plant;
  retValue->area = retValue->totalSides = 0;
  retValue->northSides = retValue->eastSides = retValue->southSides = retValue->westSides = NULL;

  return retValue;
}

void free_map(char **map, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    free(*(map + i));
  free(map);
  
}

void free_side(side *s)
{
  side *toFree;

  while (s != NULL) {
    toFree = s;
    s = s->next;
    free(toFree);
  }
}

void free_region(region *r)
{
  free_side(r->northSides);
  free_side(r->eastSides);
  free_side(r->westSides);
  free_side(r->southSides);

  free(r);
}

void walk_region(char plant, char **map, int rows, int cols, int i, int j, int *areaPerim)
{
  char c, nc, ec, sc, wc;
  int perim;

  if (i < 0 || i > rows - 1 || j < 0 || j > cols - 1)
    return;

  c = *(*(map + i) + j);

  if (c == plant) {
    perim = 0;
    nc = NORTH_CHAR(map, i, j);
    ec = EAST_CHAR(map, i, j, cols);
    sc = SOUTH_CHAR(map, i, j, rows);
    wc = WEST_CHAR(map, i, j);

    if (nc != '-' && c != nc)
      perim++;

    if (ec != '-' && c != ec)
      perim++;

    if (sc != '-' && c != sc)
      perim++;

    if (wc != '-' && c != wc)
      perim++;

    *areaPerim += 1;
    *(areaPerim + 1) += perim;

    *(*(map + i) + j) = '-';
    
    walk_region(plant, map, rows, cols, i - 1, j, areaPerim);
    walk_region(plant, map, rows, cols, i, j + 1, areaPerim);
    walk_region(plant, map, rows, cols, i + 1, j, areaPerim);
    walk_region(plant, map, rows, cols, i, j - 1, areaPerim);

    *(*(map + i) + j) = '.';
  }
}

void walk_region_2(char plant, char **map, int rows, int cols, int i, int j, region *r)
{
  char c, nc, ec, sc, wc;

  if (i < 0 || i > rows - 1 || j < 0 || j > cols - 1)
    return;

  c = *(*(map + i) + j);

  if (c == plant) {
    nc = NORTH_CHAR(map, i, j);
    ec = EAST_CHAR(map, i, j, cols);
    sc = SOUTH_CHAR(map, i, j, rows);
    wc = WEST_CHAR(map, i, j);

    if (nc != '-' && c != nc) {
      if (r->northSides == NULL)
        r->northSides = alloc_side(i, j);
      else
        add_side(r->northSides, i, j);
    }

    if (ec != '-' && c != ec) {
      if (r->eastSides == NULL)
        r->eastSides = alloc_side(j, i);
      else
        add_side(r->eastSides, j, i);
    }

    if (sc != '-' && c != sc) {
      if (r->southSides == NULL)
        r->southSides = alloc_side(i, j);
      else
        add_side(r->southSides, i, j);
    }

    if (wc != '-' && c != wc) {
      if (r->westSides == NULL)
        r->westSides = alloc_side(j, i);
      else
        add_side(r->westSides, j, i);
    }

    r->area += 1;
    *(*(map + i) + j) = '-';
    
    walk_region_2(plant, map, rows, cols, i - 1, j, r);
    walk_region_2(plant, map, rows, cols, i, j + 1, r);
    walk_region_2(plant, map, rows, cols, i + 1, j, r);
    walk_region_2(plant, map, rows, cols, i, j - 1, r);

    *(*(map + i) + j) = '.';
  }
}

void add_side(side *s, int index, int plant)
{
  while (s->index != index && s->next != NULL)
    s = s->next;

  if (s->index == index) {
    s->size += 1;
    s->plants = realloc(s->plants, s->size * sizeof(int));
    add_plant_to_side(s, plant);
  }
  else
    s->next = alloc_side(index, plant);
}

void add_plant_to_side(side *s, int plant)
{
  int size, i, j, *plants;

  plants = s->plants;
  size = s->size;
  i = 0;
  while (i < size - 1 && plant > *(plants + i))
    i++;

  if (i == size - 1)
    *(plants + i) = plant;
  else {
    j = i;
    while (j < size - 1)
      j++;
    while (j > i) {
      *(plants + j) = *(plants + (j - 1));
      j--;
    }
    *(plants + i) = plant;
  }
}

int total_sides(side *s)
{
  int retValue, i;
  retValue = 0;

  while (s != NULL) {
    retValue += 1;
    for (i = 0; i < s->size - 1; i++)
      if ( *(s->plants + i) - *(s->plants + i + 1) != -1)
        retValue += 1;
    s = s->next;
  }
  return retValue;
}

void print_map(char **map, int rows)
{
  int i;

  for (i = 0; i < rows; i++)
    printf("%s", *(map + i));
}

void print_region(region *r)
{
  printf("For region with plant %c it had area %d, total sides %d\n", r->plant, r->area, r->totalSides);
  printf("north sides:\n");
  print_sides(r->northSides);
  total_sides(r->northSides);
  printf("east sides:\n");
  print_sides(r->eastSides);
  total_sides(r->eastSides);
  printf("south sides:\n");
  print_sides(r->southSides);
  total_sides(r->southSides);
  printf("west sides:\n");
  print_sides(r->westSides);
  total_sides(r->westSides);
  printf("\n");
}

void print_sides(side *s)
{
  int i;

  while (s != NULL) {
    printf("side has index %d with size %d with plants: ", s->index, s->size);
    for (i = 0; i < s->size; i++)
      printf("%d -> ", *(s->plants + i));
    printf("NULL\n");
    s = s->next;
  }
}
