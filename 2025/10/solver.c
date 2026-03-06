#include "day_10.h"
#include "solver.h"

Z3_context mk_context(void)
{
  Z3_config cfg;
  Z3_context ctx;

  cfg = Z3_mk_config();
  Z3_set_param_value(cfg, "model", "true");
  ctx = Z3_mk_context(cfg);
  Z3_del_config(cfg);

  return ctx;
}

Z3_optimize mk_optimize(Z3_context ctx)
{
  Z3_optimize opt;

  opt = Z3_mk_optimize(ctx);
  Z3_optimize_inc_ref(ctx, opt);

  return opt;
}

Z3_ast mk_int(Z3_context ctx, int v)
{
  Z3_sort ty;

  ty = Z3_mk_int_sort(ctx);
  return Z3_mk_int(ctx, v, ty);
}

Z3_ast mk_int_var(Z3_context ctx, const char *name)
{
  Z3_symbol s;
  Z3_sort ty;

  s = Z3_mk_string_symbol(ctx, name);
  ty = Z3_mk_int_sort(ctx);
  return Z3_mk_const(ctx, s, ty);
}

Z3_ast *mk_btn_vars(Z3_context ctx, Z3_optimize opt, btns b)
{
  char *v;
  int i, vlen;
  Z3_ast zero, *retValue;

  zero = mk_int(ctx, 0);

  retValue = malloc(b.len * sizeof(Z3_ast));
  vlen = snprintf(NULL, 0, "%c%d", 'b', b.len);
  v = malloc((vlen + 1) * sizeof(char));
  *(v + vlen) = '\0';
  for (i = 0; i < b.len; i++) {
    snprintf(v, vlen + 1, "%c%d", 'b', i);
    *(retValue + i) = mk_int_var(ctx, v);
    Z3_optimize_assert(ctx, opt, Z3_mk_ge(ctx, *(retValue + i), zero));
  }

  free(v);
  return retValue;
}

unsigned mk_sum(Z3_context ctx, Z3_optimize opt, Z3_ast *vars, int len)
{
  Z3_ast sum;

  sum = mk_int_var(ctx, "sum");
  Z3_optimize_assert(ctx, opt, Z3_mk_eq(ctx,
                                        sum,
                                        Z3_mk_add(ctx, len, vars)));
  return Z3_optimize_minimize(ctx, opt, sum);
}

void mk_eqns(Z3_context ctx, Z3_optimize opt, Z3_ast *vars, btns b, jolt j)
{
  int i, ii, iexpr;
  Z3_ast *expr;

  expr = malloc(b.len * sizeof(Z3_ast));
  for (i = 0; i < j.len; i++) {
    iexpr = 0;
    for (ii = 0; ii < b.len; ii++)
      if (*(b.btns + ii) & (1 << (j.len - (i + 1))))
        *(expr + iexpr++) = *(vars + ii);
    Z3_optimize_assert(ctx, opt, Z3_mk_eq(ctx,
                                          mk_int(ctx, *(j.reqs + i)),
                                          Z3_mk_add(ctx, iexpr, expr)));
  }
  free(expr);
}

uint64_t get_sum(Z3_context ctx, Z3_optimize opt, Z3_lbool expected_result, unsigned idx)
{
  uint64_t retValue;
  Z3_model m;
  Z3_lbool result;

  printf("getting sum for:\n%s\n", Z3_optimize_to_string(ctx, opt));

  m = 0;
  result = Z3_optimize_check(ctx, opt, 0, 0);

  switch (result) {
  case Z3_L_FALSE:
    printf("unsat\n");
    break;
  case Z3_L_UNDEF:
    printf("unknown\n");
    m = Z3_optimize_get_model(ctx, opt);
    if (m) Z3_model_inc_ref(ctx, m);
    printf("potential model:\n%s\n", Z3_model_to_string(ctx, m));
    break;
  case Z3_L_TRUE:
    m = Z3_optimize_get_model(ctx, opt);
    if (m) Z3_model_inc_ref(ctx, m);
    printf("sat\n%s\n", Z3_model_to_string(ctx, m));
    break;
  }

  if (result != expected_result) printf("unexpected result\n");
  if (m) Z3_model_dec_ref(ctx, m);

  retValue = 0;
  Z3_get_numeral_uint64(ctx, Z3_optimize_get_lower(ctx, opt, idx), &retValue);
  printf("sum: %lu\n\n", retValue);
  return retValue;
}

void free_z3(Z3_context ctx, Z3_optimize opt, Z3_ast *vars)
{
  Z3_optimize_dec_ref(ctx, opt);
  Z3_del_context(ctx);
  free(vars);
  Z3_finalize_memory();
}
