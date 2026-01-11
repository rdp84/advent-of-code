#include "day_22.h"

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
  int i, buyerslen;
  long seed, secret, total, **buyers;

  buyerslen = 0;
  buyers = alloc_buyers(&buyerslen);

  total = 0;
  for (i = 0; i< buyerslen; i++) {
    seed = *(*(buyers + i));
    secret = *(*(buyers + i) + (ROUNDS - 1));
    total += secret;
    printf("seed: %ld, secret: %ld\n", seed, secret);
  }
  printf("total: %ld\n", total);
  free_buyers(buyers, buyerslen);
}

void part_2(void)
{
  int i, j, buyerslen,
    totalsmap[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN],
    buyermap[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN];
  long **buyers;
  price **prices, *p;

  buyerslen = 0;
  buyers = alloc_buyers(&buyerslen);
  prices = alloc_prices(buyers, buyerslen);

  printf("number of buyers: %d\n", buyerslen);
  for (i = 0; i < buyerslen; i++) {
    for (j = 1; j <= ROUNDS - SEQ_LEN; j++) {
      p = (*(prices + i) + j);
      if (!seen_seq(p, buyermap))
        add_to_map(p, totalsmap);
    }
    reset_map(buyermap);
  }

  print_map(totalsmap);
  find_max(totalsmap);

  free_buyers(buyers, buyerslen);
  free_prices(prices, buyerslen);
}

long step_one(long secret)
{
  return prune(mix(secret, (secret << 6)));
}

long step_two(long secret)
{
  return prune(mix(secret, (secret >> 5)));
}

long step_three(long secret)
{
  return prune(mix(secret, (secret << 11)));
}

long **alloc_buyers(int *buyers)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line;
  int i;
  long secret, *buyer, **retValue;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  
  *buyers = 0;
  retValue = NULL;
  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    retValue = realloc(retValue, (*buyers + 1) * sizeof(long *));
    *(retValue + *buyers) = malloc(ROUNDS * sizeof(long));
    buyer = *(retValue + *buyers);

    secret = atol(line);
    for (i = 0; i < ROUNDS; i++) {
      *(buyer + i) = secret;
      secret = step_three(step_two(step_one(secret)));
    }

    (*buyers)++;
    free(line);
    line = NULL;
  }

  fclose(ifp);
  return retValue;
}

void free_buyers(long **buyers, int len)
{
  int i;

  for (i = 0; i < len; i++)
    free(*(buyers + i));
  free(buyers);
}

price **alloc_prices(long **buyers, int len)
{
  int i, j;
  long secret;
  price *cur, *prev, **retValue;

  retValue = malloc(len * sizeof(price *));
  for (i = 0; i < len; i++) {
    *(retValue + i) = malloc(ROUNDS * sizeof(price));
    cur = prev = *(retValue + i);
    secret = *(*(buyers + i));

    cur->digit = (int) (secret % 10);
    cur->delta = 0;
    
    for (j = 1; j < ROUNDS; j++) {
      secret = *(*(buyers + i) + j);  
      cur = (*(retValue + i) + j);
      cur->digit = (int) (secret % 10);
      cur->delta = cur->digit - prev->digit;
      prev = cur;
    }
  }
  return retValue;
}

void print_prices(long **buyers, price **prices, int len)
{
  int i, j;
  long secret;
  price *p;

  for (i = 0; i < len; i++) {
    printf("Buyer number %d prices:\n", i);
    for (j = 0; j < ROUNDS; j++) {
      secret = *(*(buyers + i) + j);
      p = (*(prices + i) + j);
      printf("(%d) %ld: %d (%d)\n", j, secret, p->digit, p->delta);
    }
  }
}

void free_prices(price **prices, int len)
{
  int i;

  for (i = 0; i < len; i++)
    free(*(prices + i));

  free(prices);
}

void reset_map(int map[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN])
{
  int i, j, k, l;

  for (i = 0; i < KEY_LEN; i++)
    for (j = 0; j < KEY_LEN; j++)
      for (k = 0; k < KEY_LEN; k++)
        for (l = 0; l < KEY_LEN; l++)
          map[i][j][k][l] = 0;
}

bool seen_seq(price *p, int map[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN])
{
  int i, j, k, l;
  bool retValue;

  i = (p->delta) + 9;
  j = ((p + 1)->delta) + 9;
  k = ((p + 2)->delta) + 9;
  l = ((p + 3)->delta) + 9;

  retValue = map[i][j][k][l] != 0;
  map[i][j][k][l] = 1;
  
  return retValue;
}

void add_to_map(price *p, int map[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN])
{
  int i, j, k, l;

  i = (p->delta) + 9;
  j = ((p + 1)->delta) + 9;
  k = ((p + 2)->delta) + 9;
  l = ((p + 3)->delta) + 9;

  map[i][j][k][l] += ((p + 3)->digit);
}

void print_map(int map[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN])
{
  int i, j, k, l;

  for (i = 0; i < KEY_LEN; i++) {
    for (j = 0; j < KEY_LEN; j++) {
      for (k = 0; k < KEY_LEN; k++) {
        for (l = 0; l < KEY_LEN; l++) {
          printf("%d,%d,%d,%d: %d\n", (i - 9), (j - 9), (k - 9), (l - 9), map[i][j][k][l]);
        }
      }
    }
  }
}

void find_max(int map[KEY_LEN][KEY_LEN][KEY_LEN][KEY_LEN])
{
  int i, j, k, l, maxi, maxj, maxk, maxl, n, max;

  max = INT32_MIN;

  for (i = 0; i < KEY_LEN; i++) {
    for (j = 0; j < KEY_LEN; j++) {
      for (k = 0; k < KEY_LEN; k++) {
        for (l = 0; l < KEY_LEN; l++) {
          if ((n = map[i][j][k][l]) > max) {
            max = n;
            maxi = i;
            maxj = j;
            maxk = k;
            maxl = l;
          }
        }
      }
    }
  }
  printf("max seq: %d,%d,%d,%d = %d\n", (maxi - 9), (maxj-9), (maxk - 9), (maxl - 9), max);
}
