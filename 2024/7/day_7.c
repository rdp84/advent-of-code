#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void part_1(int64_t **);
void part_2(int64_t **);

int64_t **alloc_equations(void);
void print_equations(int64_t **);
void free_equations(int64_t **);

bool check_equation_part_1(int64_t *, int, char, int64_t, int64_t);
bool check_equation_part_2(int64_t *, int, char, int64_t, int64_t);
int64_t concatenate_values(int64_t, int64_t);

int main(void)
{
  int64_t **equations;

  equations = alloc_equations();
  part_1(equations);
  part_2(equations);
  free_equations(equations);

  return EXIT_SUCCESS;
}

int64_t **alloc_equations(void)
{
  FILE *ifp;
  char *line, *token, *toFree;
  size_t linecap;
  ssize_t linelen;
  int numEquations, numOperands;
  int64_t **retValue, *operands, *rows;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  retValue = malloc(sizeof(int64_t *));
  numEquations = 0;

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    toFree = line;
    numOperands = 0;
    operands = malloc(2 * sizeof(int64_t));

    token = strsep(&line, ":");
    *(operands + 1) = atoll(token);

    while ((token = strsep(&line, " ")) != NULL) {
      if (*token != '\0') {
        operands = realloc(operands, (numOperands + 3) * sizeof(int64_t));
        *(operands + (numOperands + 2)) = atoll(token);
        numOperands++;
      }
    }
    *operands = numOperands;
    retValue = realloc(retValue, (numEquations + 2) * sizeof(int64_t *));
    *(retValue + (numEquations + 1)) = operands;
    numEquations++;

    free(toFree);
    line = NULL;
  }

  fclose(ifp);
  rows = malloc(sizeof(int64_t));
  *rows = numEquations;
  *retValue = rows;

  return retValue;
}

void print_equations(int64_t **equations)
{
  int64_t *operands, numEquations, numOperands, target;

  numEquations = **equations++;
  printf("Number of equations: %lld\n", numEquations);

  while (numEquations-- > 0) {
    operands = *equations++;
    numOperands = *operands++;
    target = *operands++;
    printf("Equation has %lld operands, a target of %lld: ", numOperands, target);

    while (numOperands-- > 0)
      printf("%lld ", *operands++);
    printf("\n");
  }
}

void free_equations(int64_t **equations)
{
  int64_t length;
  int i;

  length = **equations + 1;
  for (i = 0; i < length; i++)
    free(*(equations + i));
  free(equations);  
}

void part_1(int64_t **equations)
{
  int64_t *operands, target, numEquations, numOperands, totalCalibration;

  numEquations = **equations++;
  totalCalibration = 0;

  while (numEquations-- > 0) {
    operands = *equations++;
    numOperands = *operands++;
    target = *operands++;

    if (check_equation_part_1(operands, numOperands, '*', target, 0) ||
        check_equation_part_1(operands, numOperands, '+', target, 0))
      totalCalibration += target;
  }

  printf("part 1 totalCalibration: %lld\n", totalCalibration);
}

void part_2(int64_t **equations)
{
  int64_t *operands, target, numEquations, numOperands, totalCalibration;

  numEquations = **equations++;
  totalCalibration = 0;

  while (numEquations-- > 0) {
    operands = *equations++;
    numOperands = *operands++;
    target = *operands++;

    if (check_equation_part_2(operands, numOperands, '*', target, 0) ||
        check_equation_part_2(operands, numOperands, '+', target, 0) ||
        check_equation_part_2(operands, numOperands, '|', target, 0))
      totalCalibration += target;
  }

  printf("part 2 totalCalibration: %lld\n", totalCalibration);
}

bool check_equation_part_1(int64_t *operands, int n, char operator, int64_t target, int64_t total)
{
  if (n == 0)
    return total == target;

  if (operator == '*')
    total = total == 0 ? *operands : total * *operands;
  else
    total += *operands;

  n--;
  operands++;

  return
    check_equation_part_1(operands, n, '*', target, total) ||
    check_equation_part_1(operands, n, '+', target, total);
}

bool check_equation_part_2(int64_t *operands, int n, char operator, int64_t target, int64_t total)
{
  if (n == 0)
    return total == target;

  if (operator == '|') {
    if (total == 0) {
      total = *operands;
      operands++;
      n--;
    }
    total = concatenate_values(total, *operands);
  }
  else if (operator == '*')
    total = total == 0 ? *operands : total * *operands;
  else
    total += *operands;

  n--;
  operands++;

  return
    check_equation_part_2(operands, n, '*', target, total) ||
    check_equation_part_2(operands, n, '+', target, total) ||
    check_equation_part_2(operands, n, '|', target, total);
}

int64_t concatenate_values(int64_t v1, int64_t v2)
{
  char *str;
  int length;
  int64_t retValue;

  length = snprintf(NULL, 0, "%lld%lld", v1, v2) + 1;
  str = malloc(length * sizeof(char));
  snprintf(str, length, "%lld%lld", v1, v2);
  
  retValue = atoll(str);
  free(str);
  return retValue;
}
