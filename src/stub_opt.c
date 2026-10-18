#include "tac.h"
#include <stdlib.h>
#include <string.h>

/* Temporary stub until Kalpana's DAG and CSE optimizer lands */
Quad *optimize_block(Block *b, int *n) {
    if (!b || b->n == 0) {
        *n = 0;
        return NULL;
    }
    Quad *res = (Quad *)malloc(sizeof(Quad) * b->n);
    memcpy(res, b->q, sizeof(Quad) * b->n);
    *n = b->n;
    return res;
}
