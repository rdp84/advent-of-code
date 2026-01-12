#include "day_23.h"

void part_1(void);

int main(void)
{

  part_1();
  return EXIT_SUCCESS;
}

void part_1(void)
{
  char c1[3], c2[3], c3[3];
  int i1, j1, i2, j2, i3, j3, ans;
  conns_map_t c;
  threes *t, *next, *prev;

  c1[2] = c2[2] = c3[2] = '\0';
  init_conns_map(&c);
  t = NULL;
  /* print_conns_map(&c); */

  for (i1 = 0; i1 < KEY_LEN; i1++) {
    for (j1 = 0; j1 < KEY_LEN; j1++) {
      for (i2 = 0; i2 < KEY_LEN; i2++) {
        for (j2 = 0; j2 < KEY_LEN; j2++) {
          if (c.map[i1][j1][i2][j2]) {
            for (i3 = 0; i3 < KEY_LEN; i3++) {
              for (j3 = 0; j3 < KEY_LEN; j3++) {
                if (c.map[i2][j2][i3][j3] &&
                    c.map[i3][j3][i1][j1]) {
                  c1[0] = itoc(i1);
                  c1[1] = itoc(j1);
                  c2[0] = itoc(i2);
                  c2[1] = itoc(j2);
                  c3[0] = itoc(i3);
                  c3[1] = itoc(j3);

                  if (c1[0] == 't' || c2[0] == 't' || c3[0] == 't') {
                    if (t != NULL) {
                      next = prev = t;
                      while (next != NULL) {
                        if (strstr(next->conn, c1) != NULL &&
                            strstr(next->conn, c2) != NULL &&
                            strstr(next->conn, c3) != NULL)
                          break;
                        else {
                          prev = next;
                          next = next->next;
                        }
                      }
                      if (next == NULL) {
                        prev->next = malloc(sizeof(threes));
                        prev->next->conn = malloc(9 * sizeof(char));
                        *(prev->next->conn) = '\0';
                        prev->next->conn = strcat(prev->next->conn, c1);
                        *(prev->next->conn + 2) = ',';
                        *(prev->next->conn + 3) = '\0';
                        prev->next->conn = strcat(prev->next->conn, c2);
                        *(prev->next->conn + 5) = ',';
                        *(prev->next->conn + 6) = '\0';
                        prev->next->conn = strcat(prev->next->conn, c3);
                        prev->next->next = NULL;
                      }
                    }
                    else {
                      t = malloc(sizeof(threes));
                      t->conn = malloc(9 * sizeof(char));
                      *(t->conn) = '\0';
                      t->conn = strcat(t->conn, c1);
                      *(t->conn + 2) = ',';
                      *(t->conn + 3) = '\0';
                      t->conn = strcat(t->conn, c2);
                      *(t->conn + 5) = ',';
                      *(t->conn + 6) = '\0';
                      t->conn = strcat(t->conn, c3);
                      t->next = NULL;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  ans = 0;
  next = t;
  while (next != NULL) {
    printf("set of three with: %s\n", next->conn);
    next = next->next;
    ans++;
  }
  printf("ans: %d\n", ans);
  while (t != NULL) {
    next = t;
    t = t->next;
    free(next->conn);
    free(next);
  }
}

void init_conns_map(conns_map_t *conns)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *tofree, *from, *to;
  int i, j, k, l;

  for (i = 0; i < KEY_LEN; i++)
    for (j = 0; j < KEY_LEN; j++)
      for (k = 0; k < KEY_LEN; k++)
        for (l = 0; l < KEY_LEN; l++)
          conns->map[i][j][k][l] = false;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");
  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    tofree = line;
    from = strsep(&line, "-");
    to = line;
    add_conn(from, to, conns);

    free(tofree);
    line = NULL;
  }

  fclose(ifp);
}

void add_conn(char *from, char *to, conns_map_t *conns)
{
  int ifr, jfr, ito, jto;

  ifr = ctoi(*from);
  jfr = ctoi(*(from + 1));

  ito = ctoi(*to);
  jto = ctoi(*(to + 1));

  conns->map[ifr][jfr][ito][jto] = true;
  conns->map[ito][jto][ifr][jfr] = true;
}

void print_conns_map(conns_map_t *conns)
{
  char from[3], to[3];
  int i, j, k, l;

  from[2] = to[2] = '\0';

  for (i = 0; i < KEY_LEN; i++) {
    for (j = 0; j < KEY_LEN; j++) {
      from[0] = itoc(i);
      from[1] = itoc(j);
      for (k = 0; k < KEY_LEN; k++) {
        for (l = 0; l < KEY_LEN; l++) {
          to[0] = itoc(k);
          to[1] = itoc(l);
          printf("%s-%s: %s", from, to, conns->map[i][j][k][l] ? "true\n" : "false\n");
        }
      }
    }
  }
}
