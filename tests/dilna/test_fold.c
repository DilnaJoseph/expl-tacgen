#include "../../src/tac.h"
#include "../../src/fold.h"
#include "../../src/stats.h"
#include <stdio.h>
#include <assert.h>

int main(void) {
    long res = 0;
    stats_reset();

    printf("Running Constant Folding Unit Tests...\n");

    /* 1. Normal operations */
    assert(fold_const(OP_ADD, 10, 20, &res) == 1 && res == 30);
    assert(fold_const(OP_SUB, 50, 15, &res) == 1 && res == 35);
    assert(fold_const(OP_MUL, 6, 7, &res) == 1 && res == 42);

    /* 2. C division and modulo semantics (truncate toward zero) */
    assert(fold_const(OP_DIV, 7, 2, &res) == 1 && res == 3);
    assert(fold_const(OP_DIV, -7, 2, &res) == 1 && res == -3);
    assert(fold_const(OP_DIV, 7, -2, &res) == 1 && res == -3);
    assert(fold_const(OP_DIV, -7, -2, &res) == 1 && res == 3);
    assert(fold_const(OP_MOD, 7, 3, &res) == 1 && res == 1);
    assert(fold_const(OP_MOD, -7, 3, &res) == 1 && res == -1);

    /* 3. Division and Modulo by Zero: MUST return 0 and NOT increment stats */
    int folds_before = stats.folds;
    assert(fold_const(OP_DIV, 10, 0, &res) == 0);
    assert(fold_const(OP_MOD, 10, 0, &res) == 0);
    assert(stats.folds == folds_before);

    /* 4. Relational Folding (fold_rel) */
    assert(fold_rel(REL_LT, 3, 5) == 1);
    assert(fold_rel(REL_LT, 5, 3) == 0);
    assert(fold_rel(REL_LE, 5, 5) == 1);
    assert(fold_rel(REL_GT, 10, 2) == 1);
    assert(fold_rel(REL_GE, 2, 2) == 1);
    assert(fold_rel(REL_EQ, 4, 4) == 1);
    assert(fold_rel(REL_NE, 4, 5) == 1);

    printf("All Constant Folding unit tests passed successfully!\n");
    return 0;
}
