#ifndef DAY_9_H
#define DAY_9_H 1

#include <stdio.h>
#include <stdlib.h>

struct disk_map {
  int id;
  struct disk_map *next;
  struct disk_map *prev;
};
typedef struct disk_map disk_map;

disk_map *alloc_map(char *);
void sort_map_1(disk_map *);
void sort_map_2(disk_map *);
int64_t checksum(disk_map *);
void free_map(disk_map *);
void print_map(disk_map *);

#endif
