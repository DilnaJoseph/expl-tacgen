#ifndef FOLD_H
#define FOLD_H

#include "tac.h"

/* Returns 1 if folded successfully, 0 otherwise (e.g. division by zero) */
int fold_const(Op op, long c1, long c2, long *result);

/* Constant relational folding */
int fold_rel(int relop, long c1, long c2);

#endif /* FOLD_H */
