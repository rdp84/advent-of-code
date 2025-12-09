#ifndef DAY_20_H
#define DAY_20_H 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define START   'S'
#define END     'E'
#define TRACK   '.'
#define WALL    '#'
#define VISITED 'V'

#define cur_track(track, i, j)   (*(*(track + i) + j))
#define up_track(track, i, j)    (*(*(track + i - 1) + j))
#define down_track(track, i, j)  (*(*(track + i + 1) + j))
#define right_track(track, i, j) (*(*(track + i) + j + 1))
#define left_track(track, i, j)  (*(*(track + i) + j - 1))

struct pico {
  int r;
  int c;
};
typedef struct pico pico;

char **alloc_racetrack(int *, int *, int *);
void   free_racetrack(char **, int);
void   print_racetrack(char **, int);

pico  *alloc_race(char **, int, int, int);
void   print_race(pico *, int);

int distance_to(pico, pico);
int find_cheats(pico *, int, int, int);
#endif
