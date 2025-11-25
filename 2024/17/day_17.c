#include "day_17.h"

void part_1(void);

int main(void)
{
  part_1();

  return EXIT_SUCCESS;
}

void part_1(void)
{
  char *prg, opcode, oprnd;
  int ip, reg[REGISTERS];
  output *output;

  prg = alloc_program(reg);
  ip = 0;
  output = NULL;
  printf("Input:\nRegisters - a:%d, b:%d, c:%d\nProgram - %s\n", reg[0], reg[1], reg[2], prg);

  while ((opcode = *(prg + ip)) != '\0') {
    oprnd = *(prg + ip + 1);

    switch (opcode) {
    case '0':
      adv(reg, oprnd);
      ip += 2;
      break;
    case '1':
      bxl(reg, oprnd);
      ip += 2;
      break;
    case '2':
      bst(reg, oprnd);
      ip += 2;
      break;
    case '3':
      jnz(&ip, reg, oprnd);
      break;
    case '4':
      bxc(reg);
      ip += 2;
      break;
    case '5':
      output = out(reg, oprnd, output);
      ip += 2;
      break;
    case '6':
      bdv(reg, oprnd);
      ip += 2;
      break;
    case '7':
      cdv(reg, oprnd);
      ip += 2;
      break;
    default:
      printf("ERROR: did not recognise opcode");
      free(prg);
      return;
    }
  }

  printf("\nOutput:\nRegisters - a:%d, b:%d, c:%d\n", reg[0], reg[1], reg[2]);
  print_output(output);
  free_output(output);
  free(prg);
}

char *alloc_program(int *reg)
{
  FILE *ifp;
  size_t linecap;
  ssize_t linelen;
  char *retValue, *line, *toFree, *pos, *num, chr;
  int len, i;

  line = NULL;
  linecap = 0;
  ifp = fopen("input.txt", "r");

  while ((linelen = getline(&line, &linecap, ifp)) > 1) {
    toFree = line;
    while ((chr = *(line++)) != A && chr != B && chr != C)
      ;

    line++;
    pos = line;
    while (*line++ != '\n')
      ;

    len = line - pos;
    num = malloc(len * sizeof(char));
    strlcpy(num, pos, len);

    if (chr == A)
      *reg = atoi(num);
    else if (chr == B)
      *(reg + 1) = atoi(num);
    else
      *(reg + 2) = atoi(num);

    free(num);
    free(toFree);
    line = NULL;
  }

  free(line);
  line = NULL;
  getline(&line, &linecap, ifp);
  toFree = line;
  len = 0;
  while ((chr = *line++) != '\n')
    if (chr >= '0' && chr <= '9')
      len++;

  len++;
  retValue = malloc(len * sizeof(char));
  *(retValue + len - 1) = '\0';
  line = toFree;
  i = 0;
  while ((chr = *line++) != '\n')
    if (chr >= '0' && chr <= '9')
      *(retValue + i++) = chr;
  
  free(toFree);
  fclose(ifp);
  return retValue;
}

void free_output(output *o)
{
  output *toFree;

  while (o != NULL) {
    toFree = o;
    o = o->next;
    free(toFree);
  }
}

void print_output(output *o)
{
  while (o != NULL) {
    printf("%d%s", o->n, o->next != NULL ? "," : "\n");
    o = o->next;
  }
}

void adv(int *reg, char oprnd)
{
  int den;

  if (is_literal(oprnd))
    den = pow(2, char_to_int(oprnd));
  else
    den = pow(2, *(reg + get_reg(oprnd)));

  *reg /=  den;
}

void bxl(int *reg, char oprnd)
{
  *(reg + 1) = *(reg + 1) ^ char_to_int(oprnd);
}

void bst(int *reg, char oprnd)
{
  *(reg + 1) = is_literal(oprnd) ?
    (char_to_int(oprnd)) % 8 :
    *(reg + (get_reg(oprnd))) % 8;
}

void jnz(int *ip, int *reg, char oprnd)
{
  *ip = *reg != 0 ? char_to_int(oprnd) : *ip + 2;
}

void bxc(int *reg)
{
  *(reg + 1) = *(reg + 1) ^ *(reg + 2);
}

output *out(int *reg, char oprnd, output *o)
{
  int n;
  output *retValue;

  n = (is_literal(oprnd)) ?
    (char_to_int(oprnd)) % 8 :
    *(reg + (get_reg(oprnd))) % 8;

  if (o == NULL) {
    retValue = malloc(sizeof(output));
    retValue->n = n;
    retValue->next = NULL;
  }
  else {
    retValue = o;
    while (o->next != NULL)
      o = o->next;
    o->next = malloc(sizeof(output));
    o = o->next;
    o->n = n;
    o->next = NULL;
  }

  return retValue;
}

void bdv(int *reg, char oprnd)
{
  int den;

  if (is_literal(oprnd))
    den = pow(2, char_to_int(oprnd));
  else
    den = pow(2, *(reg + get_reg(oprnd)));

  *(reg + 1) = *reg / den;
}

void cdv(int *reg, char oprnd)
{
  int den;

  if (is_literal(oprnd))
    den = pow(2, char_to_int(oprnd));
  else
    den = pow(2, *(reg + get_reg(oprnd)));

  *(reg + 2) = *reg / den;
}
