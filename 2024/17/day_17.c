#include "day_17.h"

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
  char *prg;
  int64_t reg[REGISTERS];
  output *output;

  prg = alloc_program(reg);
  printf("Input:\nRegisters - a:%lld, b:%lld, c:%lld\nProgram - %s\n", reg[0], reg[1], reg[2], prg);
  output = run_program(prg, reg);

  printf("\nOutput:\nRegisters - a:%lld, b:%lld, c:%lld\n", reg[0], reg[1], reg[2]);
  print_output(output);
  free_output(output);
  free(prg);
}

void part_2(void)
{
  char *prg, c;
  output *output;
  int i;
  int64_t reg[REGISTERS], bReg, cReg, testNum, shifted;
  stack current, next, temp;

  prg = alloc_program(reg);
  bReg = *(reg + 1);
  cReg = *(reg + 2);
  i = strlen(prg);
  output = NULL;

  init_stack(&current);
  push(0, &current);
  push(1, &current);
  push(2, &current);
  push(3, &current);
  push(4, &current);
  push(5, &current);
  push(6, &current);
  push(7, &current);

  init_stack(&next);

  while (i >= 1) {
    c = *(prg + --i);

    while (!empty(&current)) {
      testNum = pop(&current);
      *reg = testNum;

      output = run_program(prg, reg);

      if (output->n == char_to_int(c)) {
        if (i >= 1) {
          shifted = testNum << 3;
          push(shifted, &next);
          push(shifted ^ 1, &next);
          push(shifted ^ 2, &next);
          push(shifted ^ 3, &next);
          push(shifted ^ 4, &next);
          push(shifted ^ 5, &next);
          push(shifted ^ 6, &next);
          push(shifted ^ 7, &next);
        }
        else
          push(testNum, &next);
      }

      *(reg + 1) = bReg;
      *(reg + 2) = cReg;
      free_output(output);
      output = NULL;
    }
    temp = current;
    current = next;
    next = temp;
  }
  printf("minimum number: %lld\n", find_min(&current));

  free_elems(&current);
  free_elems(&next);
  free(prg);
}

char *alloc_program(int64_t *reg)
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

output *run_program(char *prg, int64_t *reg)
{
  char opcode, oprnd;
  int ip;
  output *retValue;

  ip = 0;
  retValue = NULL;

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
      retValue = out(reg, oprnd, retValue);
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
      break;
    }
  }
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

void init_stack(stack *s)
{
  s->cnt = 0;
  s->top = NULL;
}

void push(int64_t n, stack *s)
{
  elem *e;

  e = malloc(sizeof(elem));
  e->n = n;
  e->next = s->top;

  s->cnt++;
  s->top = e;
}

int64_t pop(stack *s)
{
  int64_t retValue;
  elem *e;

  retValue = s->top->n;
  e = s->top;

  s->cnt--;
  s->top = s->top->next;
  free(e);

  return retValue;
}

int64_t find_min(stack *s)
{
  elem *e;
  int64_t retValue;

  retValue = INT64_MAX;
  e = s->top;

  while (e != NULL) {
    if (e->n < retValue)
      retValue = e->n;
    e = e->next;
  }

  return retValue;
}

bool empty(const stack *s)
{
  return s->cnt == 0;
}

void print_stack(const stack *s)
{
  elem *e;

  printf("stack has size %d\n", s->cnt);
  e = s->top;

  while (e != NULL) {
    printf("%lld -> ", e->n);
    e = e->next;
  }

  printf("NULL\n");
}

void free_elems(stack *s)
{
  elem *toFree;

  while (s->top != NULL) {
    toFree = s->top;
    s->top = s->top->next;
    free(toFree);
  }
}

void adv(int64_t *reg, char oprnd)
{
  int den;

  if (is_literal(oprnd))
    den = pow(2, char_to_int(oprnd));
  else
    den = pow(2, *(reg + get_reg(oprnd)));

  *reg /=  den;
}

void bxl(int64_t *reg, char oprnd)
{
  *(reg + 1) = *(reg + 1) ^ char_to_int(oprnd);
}

void bst(int64_t *reg, char oprnd)
{
  *(reg + 1) = is_literal(oprnd) ?
    (char_to_int(oprnd)) & 7 :
    *(reg + (get_reg(oprnd))) & 7;
}

void jnz(int *ip, int64_t *reg, char oprnd)
{
  *ip = *reg != 0 ? char_to_int(oprnd) : *ip + 2;
}

void bxc(int64_t *reg)
{
  *(reg + 1) = *(reg + 1) ^ *(reg + 2);
}

output *out(int64_t *reg, char oprnd, output *o)
{
  int n;
  output *retValue;

  n = (is_literal(oprnd)) ?
    (char_to_int(oprnd)) & 7 :
    *(reg + (get_reg(oprnd))) & 7;

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

void bdv(int64_t *reg, char oprnd)
{
  int den;

  if (is_literal(oprnd))
    den = pow(2, char_to_int(oprnd));
  else
    den = pow(2, *(reg + get_reg(oprnd)));

  *(reg + 1) = *reg / den;
}

void cdv(int64_t *reg, char oprnd)
{
  int den;

  if (is_literal(oprnd))
    den = pow(2, char_to_int(oprnd));
  else
    den = pow(2, *(reg + get_reg(oprnd)));

  *(reg + 2) = *reg / den;
}
