#include "day_11.h"

void part_1(void);
void part_2(void);

static stone *headStn;

int main(void)
{
  part_1();
  /* part_2(); */
  return EXIT_SUCCESS;
}

void part_1(void)
{
  char *toFree, *input;
  stone *stn;
  int i;

  toFree = input = strdup("510613 358 84 40702 4373582 2 0 1584");
  headStn = stn = alloc_stones(input);
  print_stones(stn);

  i = 1;
  while (i <= 25) {
    blink(stn);
    stn = headStn;
    i++;
  }

  printf("number of stones: %lld\n", count_stones(stn));
  free_stones(stn);
  free(toFree);
}

void part_2(void)
{
  char *toFree, *input;
  stone *stn;
  int i;

  toFree = input = strdup("510613 358 84 40702 4373582 2 0 1584");
  headStn = stn = alloc_stones(input);
  print_stones(stn);

  i = 1;
  while (i <= 75) {
    blink(stn);
    stn = headStn;
    merge_stones(stn);
    i++;
  }

  printf("number of stones: %lld\n", count_stones(stn));
  free_stones(stn);
  free(toFree);
}

stone *alloc_stones(char *str)
{
  stone *head, *tail;
  char *token;

  head = NULL;

  if ((token = strsep(&str, " ")) != NULL) {
    head = malloc(sizeof(stone));
    head->num = atoll(token);
    head->count = 1;
    head->next = head->prev = NULL;
    tail = head;
  
    while ((token = strsep(&str, " ")) != NULL) {
      tail->next = malloc(sizeof(stone));
      tail->next->prev = tail;
      tail = tail->next;
      tail->num = atoll(token);
      tail->count = 1;
      tail->next = NULL;
    }
  }

  return head;
}

void blink(stone *head)
{
  while (head != NULL) {
    if (head->num == 0)
      head->num = 1;
    else if ((snprintf(NULL, 0, "%lld", head->num) % 2) == 0)
      split_stone(head);
    else
      head->num *= 2024;

    head = head->next;
  }
}

void split_stone(stone *stn)
{
  char *str, *half;
  int length;
  stone *prev;

  length = snprintf(NULL, 0, "%lld", stn->num);
  str = malloc((length + 1) * sizeof(char));
  snprintf(str, length + 1, "%lld", stn->num);
  half = malloc(((length / 2) + 1) * sizeof(char));
  strlcpy(half, str, ((length / 2) + 1));

  prev = malloc(sizeof(stone));
  prev->num = atoll(half);
  prev->count = stn->count;
  if (stn->prev == NULL) {
    prev->prev = NULL;
    headStn = prev;
  }
  else {
    prev->prev = stn->prev;
    stn->prev->next = prev;
  }
  prev->next = stn;
  stn->prev = prev;

  str += length / 2;
  strlcpy(half, str, ((length / 2) + 1));
  stn->num = atoll(half);

  str -= length / 2;
  free(str);
  free(half);
}

void merge_stones(stone *head)
{
  stone *cmp, *toFree;

  while (head != NULL && head->next != NULL) {
    cmp = head->next;
    while (cmp != NULL) {
      if (head->num == cmp->num) {
        head->count += cmp->count;
        cmp->prev->next = cmp->next;

        if (cmp->next != NULL)
          cmp->next->prev = cmp->prev;

        toFree = cmp;
        cmp = cmp->next;
        free(toFree);
      }
      else
        cmp = cmp->next;
    }
    head = head->next;
  }
}

int64_t count_stones(stone *head)
{
  int64_t retValue;

  retValue = 0;
  while (head != NULL) {
    retValue += head->count;
    head = head->next;
  }

  return retValue;
}

void print_stones(stone *head)
{
  if (head == NULL)
    printf("NULL\n");
  else {
    printf("%lld --> ", head->num);
    print_stones(head->next);
  }
}

void free_stones(stone *head)
{
  stone *toFree;
  while (head != NULL) {
    toFree = head;
    head = head->next;
    free(toFree);
  }
}
