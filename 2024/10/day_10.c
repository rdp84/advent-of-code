#include <stdio.h>
#include <stdlib.h>

struct visited_nine {
  int row;
  int col;
  struct visited_nine *next;
};
typedef struct visited_nine visited_nine;

struct unique_visits {
  int row;
  int col;
  int visits;
  visited_nine *nines;
  struct unique_visits *next;
};
typedef struct unique_visits unique_visits;

int calculate_score(char **, int, int);
int trailhead_score(char **, int, int, int, int, unique_visits *);
void add_unique_visit(unique_visits *, int, int);

int main(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, **map;
  int rows, cols, i, j, score;

  line = NULL;
  linecap = 0;
  ifp = fopen("input_10.txt", "r");

  linelen = getline(&line, &linecap, ifp);
  if (linelen < 0) {
    printf("error reading file");
    return EXIT_FAILURE;
  }

  cols = linelen - 1;
  printf("number of columns: %d\n", cols);

  map = malloc(sizeof(char *));
  *map = line;
  rows = 1;

  line = NULL;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    map = realloc(map, (rows + 1) * sizeof(char *));
    *(map + rows) = line;
    rows++;
    line = NULL;
  }

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      printf("%c", *(*(map + i) + j));
    }
    printf("\n");
  }

  free(line);
  fclose(ifp);

  score = calculate_score(map, rows, cols);
  printf("score: %d\n", score);

  for (i = 0; i < rows; i++)
    free(*(map + i));
  free(map);

  return EXIT_SUCCESS;
}

int calculate_score(char **map, int rows, int cols)
{
  int i, j, retValue;
  unique_visits *uniqueVisits, *head, *toAdd, *toFree;
  visited_nine *toFreeNines;

  retValue = 0;
  uniqueVisits = NULL;

  printf("rows: %d, cols: %d\n", rows, cols);
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      /* printf("looking at char %c\n", *(*(map + i) + j)); */
      if (*(*(map + i) + j) == '0') {
        printf("found trailhead at %d, %d\n", i, j);
        toAdd = malloc(sizeof(unique_visits));
        toAdd->row = i;
        toAdd->col = j;
        toAdd->visits = 0;
        toAdd->nines = NULL;
        toAdd->next = NULL;

        trailhead_score(map, i, j, rows, cols, toAdd);
        
        if (uniqueVisits == NULL)
          uniqueVisits = toAdd;
        else {
          head = uniqueVisits;
          while (head->next != NULL)
            head = head->next;
          head->next = toAdd;
        }
      }
    }
  }
  
  while (uniqueVisits != NULL) {
    printf("trailhead at %d, %d has the number of unique nine visits: %d\n", uniqueVisits->row, uniqueVisits->col, uniqueVisits->visits);
    retValue += uniqueVisits->visits;
    while (uniqueVisits->nines != NULL) {
      toFreeNines = uniqueVisits->nines;
      uniqueVisits->nines = uniqueVisits->nines->next;
      free(toFreeNines);
    }
    toFree = uniqueVisits;
    uniqueVisits = uniqueVisits->next;
    free(toFree);
  }

  return retValue;
}

int trailhead_score(char **map, int i, int j, int rows, int cols, unique_visits *uniqueVisits)
{
  char c;
  int retValue;

  retValue = 0;
  c = *(*(map + i) + j);
  
  if (c == '9') {
    /* printf("reached a 9 at %d, %d\n", i, j); */
    retValue = 1;
    add_unique_visit(uniqueVisits, i, j);
  }
  else {
    if (i > 0 &&
        c + 1 == *(*(map + (i - 1)) + j)) {
      retValue += trailhead_score(map, i - 1, j, rows, cols, uniqueVisits);
    }

    if (j < cols - 1 &&
        c + 1 == *(*(map + i) + (j + 1))) {
      retValue += trailhead_score(map, i, j + 1, rows, cols, uniqueVisits);
    }

    if (i < rows - 1 &&
        c + 1 == *(*(map + (i + 1)) + j)) {
      retValue += trailhead_score(map, i + 1, j, rows, cols, uniqueVisits);
    }

    if (j > 0 &&
        c + 1 == *(*(map + i) + (j - 1))) {
      retValue += trailhead_score(map, i, j - 1, rows, cols, uniqueVisits);
    }
  }
  return retValue;
}

void add_unique_visit(unique_visits *uniqueVisits, int i, int j)
{
  visited_nine *tmp;

  if (uniqueVisits->nines == NULL) {
    /* printf("adding unique visit at %d, %d\n", i, j); */
    uniqueVisits->visits = 1;
    uniqueVisits->nines = malloc(sizeof(visited_nine));
    uniqueVisits->nines->row = i;
    uniqueVisits->nines->col = j;
    uniqueVisits->nines->next = NULL;
  }
  else {
    tmp = uniqueVisits->nines;
    /* while (tmp->next != NULL && !(tmp->row == i && tmp->col == j)) */
    /*   tmp = tmp->next; */
    while (tmp->next != NULL)
      tmp = tmp->next;
      
    /* if (tmp->next == NULL && !(tmp->row == i && tmp->col == j)) { */
      /* printf("adding unique visit at %d, %d\n", i, j); */
      uniqueVisits->visits += 1;
      tmp->next = malloc(sizeof(visited_nine));
      tmp->next->row = i;
      tmp->next->col = j;
      tmp->next->next = NULL;
    /* } */
  }
}
