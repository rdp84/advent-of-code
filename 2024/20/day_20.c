#include "day_20.h"

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
  char **track;
  int rtrack, ctrack, racelen, time, saving;
  pico *race;

  rtrack = ctrack = racelen = 0;
  time = 2;
  saving = 100;

  track = alloc_racetrack(&rtrack, &ctrack, &racelen);
  print_racetrack(track, rtrack);

  race = alloc_race(track, rtrack, ctrack, racelen);
  print_race(race, racelen);

  printf("Number of cheats that save %d picoseconds is: %d\n",
         saving, find_cheats(race, racelen, time, saving));

  free_racetrack(track, rtrack);
  free(race);
  race = NULL;
}

void part_2(void)
{
  char **track;
  int rtrack, ctrack, racelen, time, saving;
  pico *race;

  rtrack = ctrack = racelen = 0;
  time = 20;
  saving = 100;

  track = alloc_racetrack(&rtrack, &ctrack, &racelen);
  print_racetrack(track, rtrack);

  race = alloc_race(track, rtrack, ctrack, racelen);
  print_race(race, racelen);

  printf("Number of cheats that save %d picoseconds is: %d\n",
         saving, find_cheats(race, racelen, time, saving));

  free_racetrack(track, rtrack);
  free(race);
  race = NULL;
}

char **alloc_racetrack(int *rtrack, int *ctrack, int *racelen)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char c, *line, **retValue;
  int i;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  retValue = NULL;
  i = 0;
  *racelen = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    retValue = realloc(retValue, (i + 1) * sizeof(char *));
    *(retValue + i) = line;
    i++;

    while ((c = *line++))
      if (c == TRACK || c == START || c == END)
        (*racelen)++;

    line = NULL;
  }

  fclose(ifp);
  *rtrack = i;
  *ctrack = strlen(*retValue) - 1;
  for (i = 0; i < *rtrack; i++)
    *(*(retValue + i) + *ctrack) = '\0';

  return retValue;
}

void free_racetrack(char **track, int rtrack)
{
  int i;

  for (i = 0; i < rtrack; i++)
    free(*(track + i));
  free(track);
}

void print_racetrack(char **track, int rtrack)
{
  int i;

  for (i = 0; i < rtrack; i++)
    printf("%s\n", *(track + i));
}

pico *alloc_race(char **track, int rtrack, int ctrack, int racelen)
{
  char c;
  int i, j, irace;
  bool foundStart, foundEnd;
  pico *retValue;

  retValue = malloc(racelen * sizeof(pico));

  foundStart = foundEnd = false;
  for (i = 0; i < rtrack && !(foundStart && foundEnd); i++) {
    for (j = 0; j < ctrack; j++) {
      if ((c = cur_track(track, i, j)) == START) {
        (*retValue).r = i;
        (*retValue).c = j;
        foundStart = true;
      } else if (c == END) {
        (*(retValue + racelen - 1)).r = i;
        (*(retValue + racelen - 1)).c = j;
        foundEnd = true;
      } else
        continue;
    }
  }

  i = (*retValue).r;
  j = (*retValue).c;
  irace = 1;
  while (irace < racelen - 1) {
    cur_track(track, i, j) = VISITED;

    if (up_track(track, i, j) == TRACK)
      i--;
    else if (down_track(track, i, j) == TRACK)
      i++;
    else if (right_track(track, i, j) == TRACK)
      j++;
    else
      j--;

    (*(retValue + irace)).r = i;
    (*(retValue + irace)).c = j;
    irace++;
  }

  return retValue;
}

void print_race(pico *race, int racelen)
{
  int i;

  printf("length of race: %d\n", racelen);
  for (i = 0; i < racelen; i++)
    printf("pico at time %d: (%d, %d)\n", i, (*(race + i)).r, (*(race + i)).c);
}

int distance_to(pico start, pico end)
{
  return abs((start.c - end.c)) + abs((start.r - end.r));
}

int find_cheats(pico *race, int racelen, int time, int saving)
{
  int i, j, distance, retValue;
  pico start, end;

  retValue = 0;
  for (i = 0; i < racelen; i++) {
    start = *(race + i);
    for (j = i + saving; j < racelen; j++) {
      end = *(race + j);
      distance = distance_to(start, end);
      if ((distance <= time) &&
          (distance <= (j - i - saving)))
        retValue++;
    }
  }

  return retValue;
}
