#if !defined SOLVER_H
#define SOLVER_H 1

#include <z3.h>
#include "day_10.h"

Z3_context  mk_context(void);
Z3_optimize mk_optimize(Z3_context);
Z3_ast      mk_int(Z3_context, int);
Z3_ast      mk_int_var(Z3_context, const char *);
Z3_ast     *mk_btn_vars(Z3_context, Z3_optimize, btns);
unsigned    mk_sum(Z3_context, Z3_optimize, Z3_ast *, int);
void        mk_eqns(Z3_context, Z3_optimize, Z3_ast *, btns, jolt);
uint64_t    get_sum(Z3_context, Z3_optimize, Z3_lbool, unsigned);
void        free_z3(Z3_context, Z3_optimize, Z3_ast *);

#endif
