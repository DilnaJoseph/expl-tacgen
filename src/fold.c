#include "fold.h"
#include "stats.h"
#include <stdio.h>

int fold_const(Op op, long c1, long c2, long *result) {
    if (!result) return 0;

    switch (op) {
        case OP_ADD:
            *result = c1 + c2;
            stats.folds++;
            return 1;
        case OP_SUB:
            *result = c1 - c2;
            stats.folds++;
            return 1;
        case OP_MUL:
            *result = c1 * c2;
            stats.folds++;
            return 1;
        case OP_DIV:
            if (c2 == 0) {
                fprintf(stderr, "Warning: Division by zero encountered during constant folding.\n");
                return 0;
            }
            /* C semantics truncate toward zero */
            *result = c1 / c2;
            stats.folds++;
            return 1;
        case OP_MOD:
            if (c2 == 0) {
                fprintf(stderr, "Warning: Modulo by zero encountered during constant folding.\n");
                return 0;
            }
            *result = c1 % c2;
            stats.folds++;
            return 1;
        default:
            return 0;
    }
}

int fold_rel(int relop, long c1, long c2) {
    switch (relop) {
        case REL_LT: return c1 < c2;
        case REL_GT: return c1 > c2;
        case REL_LE: return c1 <= c2;
        case REL_GE: return c1 >= c2;
        case REL_EQ: return c1 == c2;
        case REL_NE: return c1 != c2;
        default:     return 0;
    }
}
