#include "day_9.h"

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
  FILE *ifp;
  size_t linecap;
  char *line;
  disk_map *map;
  int64_t sum;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  getline(&line, &linecap, ifp);

  map = alloc_map(line);
  sort_map_1(map);
  sum = checksum(map);
  printf("part 1 checksum: %lld\n", sum);

  free_map(map);
  free(line);
  fclose(ifp);
}

void part_2(void)
{
  FILE *ifp;
  size_t linecap;
  char *line;
  disk_map *map;
  int64_t sum;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  getline(&line, &linecap, ifp);

  map = alloc_map(line);
  sort_map_2(map);
  sum = checksum(map);
  printf("part 2 checksum: %lld\n", sum);

  free_map(map);
  free(line);
  fclose(ifp);
}

disk_map *alloc_map(char *str)
{
  int i, fileId;
  disk_map *head = NULL, *tail;

  i = fileId = 0;

  if (*str != '\0' && *str != '\n') {
    head = malloc(sizeof(disk_map));
    head->id = i % 2 == 0 ? fileId : -1;
    head->prev = NULL;
    tail = head;

    *str = *str - 1;
    while (*str != '\0' && *str != '\n') {
      if (*str == '0') {
        if (i % 2 == 0)
          fileId++;
        i++;
        str++;
      }
      else {
        tail->next = malloc(sizeof(disk_map));
        tail->next->id = i % 2 == 0 ? fileId : -1;
        tail->next->next = NULL;
        tail->next->prev = tail;
        tail = tail->next;
        *str = *str - 1;
      }
    }
  }
  return head;
}

void sort_map_1(disk_map *head)
{
  disk_map *tail;
  int i, j;
  
  i = j = 0;
  tail = head;
  while (tail->next != NULL) {
    tail = tail->next;
    j++;
  }

  while (tail->id == -1) {
    tail = tail->prev;
    j--;
  }

  while (i < j) {
    if (head->id == -1) {
      if (tail->id == -1)
        tail = tail->prev;
      else {
        head->id = tail->id;
        tail->id = -1;

        head = head->next;
        tail = tail->prev;

        i++;
      }
      j--;
    }
    else {
      head = head->next;
      i++;
    }
  }
}

void sort_map_2(disk_map *head)
{
  disk_map *freeStart, *freeEnd, *fileStart, *fileEnd;
  int fileLength, freeLength;

  fileEnd = head;
  while (fileEnd->next != NULL)
    fileEnd = fileEnd->next;

  while (fileEnd->id == -1)
    fileEnd = fileEnd->prev;

  fileStart = fileEnd;
  while (fileEnd->prev != NULL) {
    fileLength = 0;
    while (fileStart->id == fileEnd->id && fileStart->prev != NULL) {
      fileStart = fileStart->prev;
      fileLength++;
    }

    if (fileStart->prev == NULL)
      break;

    freeStart = freeEnd = head;
    while (freeStart->next != NULL) {
      freeLength = 0;
      while (freeStart->id != -1 && freeStart->id != fileEnd->id && freeStart->next != NULL)
        freeStart = freeEnd = freeStart->next;

      if (freeStart->id == -1) {
        while (freeEnd->id == -1 && freeEnd->next != NULL && freeLength != fileLength) {
          freeEnd = freeEnd->next;
          freeLength++;
        }

        if (freeLength == fileLength) {
          while (fileLength-- > 0) {
            freeStart->id = fileEnd->id;
            fileEnd->id = -1;
            freeStart = freeStart->next;
            fileEnd = fileEnd->prev;
          }
          break;
        }
        else if (freeEnd->id == fileEnd->id)
          break;
        else
          freeStart = freeEnd;
      }
      else
        break;
    }

    while (fileStart->id == -1 && fileStart->prev != NULL)
      fileStart = fileStart->prev;
    fileEnd = fileStart;
  }
}

int64_t checksum(disk_map *map)
{
  int i;
  int64_t retValue;

  i = 0;
  retValue = 0;
  while (map != NULL) {
    if (map->id != -1)
      retValue += map->id * i;
    i++;
    map = map->next;
  }
  return retValue;
}

void free_map(disk_map *map)
{
  if (map != NULL) {
    disk_map *next = map->next;
    free(map);
    free_map(next);
  }
}

void print_map(disk_map *map)
{
  while (map != NULL) {
    printf("< id: %d > --> ", map->id);
    map = map->next;
  }
  printf("NULL\n");
}
