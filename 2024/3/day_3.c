#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <stdbool.h>

void part_1(void);
void part_2(void);
static int get_operand(char *, regmatch_t);

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
  ssize_t linelen;
  char *line, *toFree;
  regex_t regex;
  regmatch_t matches[3];
  int left, right, total, totalMultiplications;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  if (regcomp(&regex, "mul\\(([0-9]+),([0-9]+)\\)", REG_EXTENDED | REG_ICASE) != 0) {
    printf("RegEx compilation error");
    return;
  }

  total = totalMultiplications = 0;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;

    while (regexec(&regex, line, 3, matches, 0) == 0) {
      left = get_operand(line, matches[1]);
      right = get_operand(line, matches[2]);

      total += left * right;
      totalMultiplications++;
      line += matches[0].rm_eo;
    }

    free(toFree);
    line = NULL;
    linecap = 0;
  }
  printf("totalMultiplications is: %d, total is: %d\n", totalMultiplications, total);  
  regfree(&regex);
  fclose(ifp);
}

void part_2(void)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *line, *toFree;
  regex_t regex;
  regmatch_t matches[6];
  int left, right, total, totalMultiplications;
  bool isDo;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  if (regcomp(&regex, "(mul)\\(([0-9]+),([0-9]+)\\)|(don\\'t)\\(\\)|(do)\\(\\)", REG_EXTENDED | REG_ICASE) != 0) {
    printf("RegEx compilation error");
    return;
  }

  total = totalMultiplications = 0;
  isDo = true;
  while ((linelen = getline(&line, &linecap, ifp)) > 0) {
    toFree = line;

    while (regexec(&regex, line, 6, matches, 0) == 0) {
      if (matches[1].rm_so >= 0 && isDo) {
        left = get_operand(line, matches[2]);
        right = get_operand(line, matches[3]);
        total += left * right;
        totalMultiplications++;
      } else if (matches[4].rm_so >= 0) {
        isDo = false;
      } else if (matches[5].rm_so >= 0) {
        isDo = true;
      }
      line += matches[0].rm_eo;
    }

    free(toFree);
    line = NULL;
    linecap = 0;
  }
  printf("totalMultiplications is: %d, total is: %d\n", totalMultiplications, total);
  regfree(&regex);
  fclose(ifp);
}


static int get_operand(char *line, regmatch_t match)
{
  char *operand;
  regoff_t length;
  int retValue;

  length = match.rm_eo - match.rm_so;
  operand = malloc((length + 1) * sizeof(char));
  strncpy(operand, line + match.rm_so, length);
  *(operand + length) = '\0';

  retValue = atoi(operand);
  free(operand);
  return retValue;
}
