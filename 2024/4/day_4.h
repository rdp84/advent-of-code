#ifndef DAY_4_H
#define DAY_4_H

#define WORD_LENGTH 4
#define X 'X'
#define M 'M'
#define A 'A'
#define S 'S'

#define check_x(i, j, wordSearch) *(*(wordSearch + i) + j) == X
#define check_m(i, j, wordSearch) *(*(wordSearch + i) + j) == M
#define check_a(i, j, wordSearch) *(*(wordSearch + i) + j) == A
#define check_s(i, j, wordSearch) *(*(wordSearch + i) + j) == S

#define check_north(i, j, wordSearch, appearances) \
  if (check_m(i - 1, j, wordSearch) &&             \
      check_a(i - 2, j, wordSearch) &&             \
      check_s(i - 3, j, wordSearch))               \
    appearances++                                  \

#define check_north_east(i, j, wordSearch, appearances) \
  if (check_m(i - 1, j + 1, wordSearch) &&              \
      check_a(i - 2, j + 2, wordSearch) &&              \
      check_s(i - 3, j + 3, wordSearch))                \
    appearances++                                       \

#define check_east(i, j, wordSearch, appearances) \
  if (check_m(i, j + 1, wordSearch) &&            \
      check_a(i, j + 2, wordSearch) &&            \
      check_s(i, j + 3, wordSearch))              \
    appearances++                                 \

#define check_south_east(i, j, wordSearch, appearances) \
  if (check_m(i + 1, j + 1, wordSearch) &&              \
      check_a(i + 2, j + 2, wordSearch) &&              \
      check_s(i + 3, j + 3, wordSearch))                \
    appearances++                                       \

#define check_south(i, j, wordSearch, appearances) \
  if (check_m(i + 1, j, wordSearch) &&             \
      check_a(i + 2, j, wordSearch) &&             \
      check_s(i + 3, j, wordSearch))               \
    appearances++                                  \

#define check_south_west(i, j, wordSearch, appearances) \
  if (check_m(i + 1, j - 1, wordSearch) &&              \
      check_a(i + 2, j - 2, wordSearch) &&              \
      check_s(i + 3, j - 3, wordSearch))                \
    appearances++                                       \

#define check_west(i, j, wordSearch, appearances) \
  if (check_m(i, j - 1, wordSearch) &&            \
      check_a(i, j - 2, wordSearch) &&            \
      check_s(i, j - 3, wordSearch))              \
    appearances++                                 \

#define check_north_west(i, j, wordSearch, appearances) \
  if (check_m(i - 1, j - 1, wordSearch) &&              \
      check_a(i - 2, j - 2, wordSearch) &&              \
      check_s(i - 3, j - 3, wordSearch))                \
    appearances++                                       \

#endif
