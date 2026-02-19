#include "day_9.h"

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
  input in;

  in = alloc_input();
  print_input(in);
  printf("ans: %ld\n", (*(in.pairs + in.npairs - 1)).area);
  free_input(in);
}

void part_2(void)
{
  input in;
  int i;
  pair p;

  in = alloc_input();
  print_input(in);

  for (i = in.npairs - 1; i >= 0; i--)
    if ((*(in.pairs + i)).redgreen)
      break;

  p = *(in.pairs + i);
  printf("largest redgreen is pair {(%d, %d), (%d, %d)} with area %ld\n", p.rt1.col, p.rt1.row, p.rt2.col, p.rt2.row, p.area);

  free_input(in);
}

input alloc_input(void)
{
  input retValue;

  retValue.nrtiles = retValue.npairs = 0;
  retValue.rtiles = get_rtiles(&retValue.nrtiles);
  retValue.pairs = get_pairs(retValue.rtiles, retValue.nrtiles, &retValue.npairs);

  return retValue;
}

tile *get_rtiles(int *nrtiles)
{
  FILE *ifp;
  size_t linecap;
  char *line, *start, delim[] = ",";
  tile t;
  tile *retValue;

  ifp = fopen("input.txt", "r");
  linecap = 0;
  line = NULL;
  *nrtiles = 0;
  while (getline(&line, &linecap, ifp) > 1)
    (*nrtiles)++;
  retValue = malloc(*nrtiles * sizeof(tile));

  rewind(ifp);
  while (getline(&line, &linecap, ifp) > 1) {
    start = line;
    t.col = atoi(strsep(&line, delim));
    t.row = atoi(line);
    *retValue++ = t;
    line = start;
  }
  free(line);
  fclose(ifp);

  retValue -= *nrtiles;
  return retValue;
}

pair *get_pairs(tile *rtiles, int nrtiles, int *npairs)
{
  int i, j;
  pair p, *retValue;
  rectangle r;

  *npairs = (nrtiles * (nrtiles - 1)) / 2;
  retValue = malloc(*npairs * sizeof(pair));

  for (i = 0; i < nrtiles - 1; i++) {
    p.rt1 = *(rtiles + i);
    for (j = i + 1; j < nrtiles; j++) {
      p.rt2 = *(rtiles + j);
      p.area = (long) (abs(p.rt1.col - p.rt2.col) + 1) * (long) (abs(p.rt1.row - p.rt2.row) + 1);
      r = make_rectangle(p.rt1, p.rt2);
      p.redgreen = rectangle_in_polygon(rtiles, nrtiles, r);
      *retValue++ = p;
    }
  }
  retValue -= *npairs;
  qsort(retValue, *npairs, sizeof(pair), pair_cmp);

  return retValue;
}

void free_input(input in)
{
  free(in.rtiles);
  free(in.pairs);
}

void print_input(input in)
{
  int i;
  tile rt;
  pair p;

  for (i = 0; i < in.nrtiles; i++) {
    rt = *(in.rtiles + i);
    printf("red tile at (%d, %d)\n", rt.col, rt.row);
  }

  printf("\n");
  for (i = 0; i < in.npairs; i++) {
    p = *(in.pairs + i);
    printf("pair {(%d, %d), (%d, %d)} is redgreen: %s and has area: %ld\n", p.rt1.col, p.rt1.row, p.rt2.col, p.rt2.row, p.redgreen ? "true" : "false", p.area);
  }
}

rectangle make_rectangle(tile t1, tile t2)
{
  sorted sc, sr;
  rectangle retValue;

  sc = sort(t1.col, t2.col);
  sr = sort(t1.row, t2.row);

  retValue.tl.col = retValue.bl.col = sc.min;
  retValue.tr.col = retValue.br.col = sc.max;
  retValue.tl.row = retValue.tr.row = sr.min;
  retValue.bl.row = retValue.br.row = sr.max;

  return retValue;
}

void print_rectangle(rectangle r)
{
  printf("rectangle {(%d, %d), (%d, %d), (%d, %d), (%d, %d)}\n", r.tl.col, r.tl.row, r.tr.col, r.tr.row, r.br.col, r.br.row, r.bl.col, r.bl.row);
}

bool rectangle_in_polygon(tile *rtiles, int nrtiles, rectangle r)
{
  if (tile_in_polygon(rtiles, nrtiles, r.tl) &&
      tile_in_polygon(rtiles, nrtiles, r.tr) &&
      tile_in_polygon(rtiles, nrtiles, r.br) &&
      tile_in_polygon(rtiles, nrtiles, r.bl) &&
      !polygon_edge_in_rectangle(rtiles, nrtiles, r)) {

    if (((r.tr.col - r.tl.col) > 1) &&
        ((r.bl.row - r.tl.row) > 1)) {
      shrink_rectangle(r);
      return tile_in_polygon(rtiles, nrtiles, r.tl) &&
        tile_in_polygon(rtiles, nrtiles, r.tr) &&
        tile_in_polygon(rtiles, nrtiles, r.br) &&
        tile_in_polygon(rtiles, nrtiles, r.bl);
        }
    else
      return true;
  }
  else
    return false;
}

bool tile_in_polygon(tile *rtiles, int nrtiles, tile t)
{
  int i;
  bool inside;
  tile rt1, rt2;
  sorted sc, sr;

  inside = false;
  for (i = 0; i < nrtiles; i++) {
    rt1 = *(rtiles + i);
    rt2 = *(rtiles + ((i + 1) % nrtiles));

    if (on_edge(rt1, rt2, t))
      return true;
    else if (rt1.row != rt2.row) {
      sc = sort(rt1.col, rt2.col);
      sr = sort(rt1.row, rt2.row);
      if (t.col < sc.min && t.row > sr.min && t.row <= sr.max)
        inside = !inside;
    }
  }
  return inside;
}

bool on_edge(tile rt1, tile rt2, tile t)
{
  sorted sc, sr;

  sc = sort(rt1.col, rt2.col);
  sr = sort(rt1.row, rt2.row);
  return t.col >= sc.min && t.col <= sc.max && t.row >= sr.min && t.row <= sr.max;
}

bool polygon_edge_in_rectangle(tile *rtiles, int nrtiles, rectangle r)
{
  int i;
  tile rt1, rt2;
  sorted sc, sr;

  for (i = 0; i < nrtiles; i++) {
    rt1 = *(rtiles + i);
    rt2 = *(rtiles + ((i + 1) % nrtiles));
    sc = sort(rt1.col, rt2.col);
    sr = sort(rt1.row, rt2.row);
    if (sc.min < r.tr.col && sc.max > r.tl.col &&
        sr.min < r.bl.row && sr.max > r.tl.row)
      return true;
  }
  return false;
}

sorted sort(int x, int y) {
  sorted retValue;

  if (x < y) {
    retValue.min = x;
    retValue.max = y;
  } else {
    retValue.min = y;
    retValue.max = x;
  }
  return retValue;
}

int pair_cmp(const void *va, const void *vb)
{
  const pair *a, *b;
  long diff;

  a = va;
  b = vb;
  diff = a->area - b->area;

  return ((diff >= 0) ? ((diff > 0) ? 1 : 0) : -1);
}
