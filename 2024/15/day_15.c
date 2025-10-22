#include "day_15.h"

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
  int i, j, iMap, jMap, iMoves, jMoves, iRobot, jRobot;
  int64_t gpsTotal;
  char c, **map, **moves;

  iMap = jMap = iMoves = jMoves = iRobot = jRobot = 0;
  gpsTotal = 0;
  map = alloc_map(&iMap, &jMap);
  moves = alloc_moves(&iMoves, &jMoves);

  print_input(map, iMap, jMap);
  print_input(moves, iMoves, jMoves);

  for (i = 0; i < iMap; i++) {
    for (j = 0; j < jMap; j++) {
      if (char_at_index(map, i, j) == ROBOT) {
        iRobot = i;
        jRobot = j;
        break;
      }
    }
  }

  printf("robot initially at index (%d, %d)\n", iRobot, jRobot);

  for (i = 0; i < iMoves; i++) {
    for (j = 0; j < jMoves; j++) {
      c = char_at_index(moves, i, j);
      if (c == NORTH)
        move(map, iRobot, jRobot, -1, 0, &iRobot, &jRobot);
      else if (c == EAST)
        move(map, iRobot, jRobot, 0, 1, &iRobot, &jRobot);
      else if (c == SOUTH)
        move(map, iRobot, jRobot, 1, 0, &iRobot, &jRobot);
      else
        move(map, iRobot, jRobot, 0, -1, &iRobot, &jRobot );
    }
  }

  printf("Map after making all moves:\n");
  print_input(map, iMap, jMap);  

  for (i = 0; i < iMap; i++) {
    for (j = 0; j < jMap; j++) {
      if (char_at_index(map, i, j) == BOX) {
        gpsTotal += (100 * i) + j;
      }
    }
  }

  printf("gpsTotal: %lld\n", gpsTotal);
  
  free_input(map, iMap);
  free_input(moves, iMoves);
}

void part_2(void)
{
  int i, j, iMap, jMap, iMoves, jMoves, iRobot, jRobot;
  int64_t gpsTotal;
  char c, **map, **wideMap, **moves;
  brackets brks;

  iMap = jMap = iMoves = jMoves = iRobot = jRobot = 0;
  gpsTotal = 0;
  initialize(&brks);

  map = alloc_map(&iMap, &jMap);
  moves = alloc_moves(&iMoves, &jMoves);

  wideMap = widen_map(map, iMap, jMap);
  jMap = jMap * 2;

  print_input(wideMap, iMap, jMap);
  print_input(moves, iMoves, jMoves);

  free_input(map, iMap);
  
  for (i = 0; i < iMap; i++) {
    for (j = 0; j < jMap; j++) {
      if (char_at_index(wideMap, i, j) == ROBOT) {
        iRobot = i;
        jRobot = j;
        break;
      }
    }
  }

  printf("robot initially at index (%d, %d)\n", iRobot, jRobot);

  for (i = 0; i < iMoves; i++) {
    for (j = 0; j < jMoves; j++) {
      c = char_at_index(moves, i, j);
      if (c == NORTH)
        vertical_move(wideMap, iRobot, jRobot, -1, &iRobot, &brks);
      else if (c == EAST)
        move(wideMap, iRobot, jRobot, 0, 1, &iRobot, &jRobot);
      else if (c == SOUTH)
        vertical_move(wideMap, iRobot, jRobot, 1, &iRobot, &brks);
      else
        move(wideMap, iRobot, jRobot, 0, -1, &iRobot, &jRobot);
    }
  }

  printf("\nMap after making all moves:\n");
  print_input(wideMap, iMap, jMap);

  for (i = 0; i < iMap; i++) {
    for (j = 0; j < jMap; j++) {
      if (char_at_index(wideMap, i, j) == LEFT_BRACKET) {
        gpsTotal += (100 * i) + j;
      }
    }
  }

  printf("gpsTotal: %lld\n", gpsTotal);
  
  free_input(wideMap, iMap);
  free_input(moves, iMoves);
}

char **alloc_map(int *rows, int *cols)
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
  ifp = fopen("input.txt", "r");

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

char **widen_map(char **map, int i, int j)
{
  char c, **retValue;
  int ii, jj;

  retValue = malloc(i * sizeof(char *));

  for (ii = 0; ii < i; ii++) {
    *(retValue + ii) = malloc(2 * j * sizeof(char));
    for (jj = 0; jj < j; jj++) {
      c = char_at_index(map, ii, jj);

      if (c == BOX) {
        char_at_index(retValue, ii, (2 * jj)) = LEFT_BRACKET;
        char_at_index(retValue, ii, ((2 * jj) + 1)) = RIGHT_BRACKET;
      } else if (c == ROBOT) {
        char_at_index(retValue, ii, (2 * jj)) = c;
        char_at_index(retValue, ii, ((2 * jj) + 1)) = FREE;
      } else {
        char_at_index(retValue, ii, (2 * jj)) = c;
        char_at_index(retValue, ii, ((2 * jj) + 1)) = c;
      }
    }
  }
  return retValue;
}

void move(char **map, int i, int j, int iDir, int jDir, int *iRobot, int *jRobot)
{
  char from, to;
  int iTo, jTo;

  from = char_at_index(map, i, j);

  if (from == ROBOT || from == BOX || from == LEFT_BRACKET || from == RIGHT_BRACKET) {
    iTo = i + iDir;
    jTo = j + jDir;

    if ((to = char_at_index(map, iTo, jTo)) == FREE) {
      char_at_index(map, i, j) = FREE;
      char_at_index(map, iTo, jTo) = from;

      if (from == ROBOT) {
        *iRobot = iTo;
        *jRobot = jTo;
      }
    } else if (to == BOX || to == LEFT_BRACKET || to == RIGHT_BRACKET) {
      move(map, iTo, jTo, iDir, jDir, iRobot, jRobot);

      if (char_at_index(map, iTo, jTo) == FREE) {
        char_at_index(map, i, j) = FREE;
        char_at_index(map, iTo, jTo) = from;

        if (from == ROBOT) {
          *iRobot = iTo;
          *jRobot = jTo;
        }
      }
    } else
      return;
  } else
    return;
}

void vertical_move(char **map, int i, int j, int iDir, int *iRobot, brackets *brks)
{
  char from, adjFrom, to, adjTo;
  int iTo, jTo, jDir;

  iTo = jTo = jDir = 0;
  from = char_at_index(map, i, j);

  if (from == ROBOT) {
    iTo = i + iDir;

    if ((to = char_at_index(map, iTo, j)) == FREE) {
      char_at_index(map, i, j) = FREE;
      char_at_index(map, iTo, j) = from;
      *iRobot = iTo;
      free_brackets(brks);
    } else if (to == LEFT_BRACKET || to == RIGHT_BRACKET) {
      vertical_move(map, iTo, j, iDir, iRobot, brks);

      if (char_at_index(map, iTo, j) == FREE) {
        char_at_index(map, i, j) = FREE;
        char_at_index(map, iTo, j) = from;
        *iRobot = iTo;
        free_brackets(brks);
      } else
        undo_moved_brackets(map, brks);
    } else
      return;
  } else if (from == WALL)
    return;
  else {
    iTo = i + iDir;

    if (from == '[') {
      jTo = j + 1;
      jDir = 1;
      adjFrom = ']';
    } else {
      jTo = j - 1;
      jDir = -1;
      adjFrom = '[';
    }

    to = char_at_index(map, iTo, j);
    adjTo = char_at_index(map, iTo, jTo);

    if (to == FREE) {
      if (adjTo == FREE) {
        char_at_index(map, iTo, j) = from;
        char_at_index(map, iTo, jTo) = adjFrom;
        char_at_index(map, i, j) = char_at_index(map, i, jTo) = FREE;

        save_bracket(from, i, j, iTo, j, brks);
        save_bracket(adjFrom, i, jTo, iTo, jTo, brks);
      } else if (adjTo == WALL)
        return;
      else {
        vertical_move(map, iTo, jTo, iDir, iRobot, brks);

        if (char_at_index(map, iTo, jTo) == FREE) {
          char_at_index(map, iTo, j) = from;
          char_at_index(map, iTo, jTo) = adjFrom;
          char_at_index(map, i, j) = char_at_index(map, i, jTo) = FREE;

          save_bracket(from, i, j, iTo, j, brks);
          save_bracket(adjFrom, i, jTo, iTo, jTo, brks);
        }
      }
    } else if (to == WALL)
      return;
    else if (to == from) {
      vertical_move(map, iTo, j, iDir, iRobot, brks);

      if (char_at_index(map, iTo, j) == FREE) {
        char_at_index(map, iTo, j) = from;
        char_at_index(map, iTo, jTo) = adjFrom;
        char_at_index(map, i, j) = char_at_index(map, i, jTo) = FREE;

        save_bracket(from, i, j, iTo, j, brks);
        save_bracket(adjFrom, i, jTo, iTo, jTo, brks);
      }
    } else {
      if (adjTo == FREE) {
        vertical_move(map, iTo, j, iDir, iRobot, brks);

        if (char_at_index(map, iTo, j) == FREE) {
          char_at_index(map, iTo, j) = from;
          char_at_index(map, iTo, jTo) = adjFrom;
          char_at_index(map, i, j) = char_at_index(map, i, jTo) = FREE;

          save_bracket(from, i, j, iTo, j, brks);
          save_bracket(adjFrom, i, jTo, iTo, jTo, brks);
        }
      } else if (adjTo == WALL)
        return;
      else {
        vertical_move(map, iTo, j, iDir, iRobot, brks);
        vertical_move(map, iTo, jTo, iDir, iRobot, brks);

        if (char_at_index(map, iTo, j) == FREE) {
          if (char_at_index(map, iTo, jTo) == FREE) {
            char_at_index(map, iTo, j) = from;
            char_at_index(map, iTo, jTo) = adjFrom;
            char_at_index(map, i, j) = char_at_index(map, i, jTo) = FREE;

            save_bracket(from, i, j, iTo, j, brks);
            save_bracket(adjFrom, i, jTo, iTo, jTo, brks);
          } else
            return;
        } else
          return;
      }
    }
  }
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

void initialize(brackets *brks)
{
  brks->cnt = 0;
  brks->top = NULL;
}

void save_bracket(char b, int iFrom, int jFrom, int iTo, int jTo, brackets *brks)
{
  bracket brk;
  elem *p;

  brk.b = b;
  brk.iFrom = iFrom;
  brk.jFrom = jFrom;
  brk.iTo = iTo;
  brk.jTo = jTo;

  p = malloc(sizeof(elem));
  p->b = brk;
  p->next = brks->top;
  brks->top = p;
  brks->cnt++;
}

bracket pop_brackets(brackets *brks)
{
  bracket b;
  elem *e;

  b = brks->top->b;
  e = brks->top;

  brks->cnt--;
  brks->top = brks->top->next;
  free(e);

  return b;
}

void free_brackets(brackets *brks)
{
  while (brks->cnt != 0)
    pop_brackets(brks);
}

void undo_moved_brackets(char **map, brackets *brks)
{
  bracket b;

  while (brks->cnt != 0) {
    b = pop_brackets(brks);
    char_at_index(map, b.iFrom, b.jFrom) = b.b;
    char_at_index(map, b.iTo, b.jTo) = FREE;
  }
}
