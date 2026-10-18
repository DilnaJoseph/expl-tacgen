#include "tac.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static int temp_counter = 1;
static int label_counter = 1;

Operand mk_none(void) {
    Operand op;
    memset(&op, 0, sizeof(Operand));
    op.kind = A_NONE;
    return op;
}

Operand mk_const(long val) {
    Operand op = mk_none();
    op.kind = A_CONST;
    op.val = val;
    snprintf(op.name, sizeof(op.name), "%ld", val);
    return op;
}

Operand mk_var(const char *name) {
    Operand op = mk_none();
    op.kind = A_VAR;
    if (name) {
        strncpy(op.name, name, sizeof(op.name) - 1);
    }
    return op;
}

Operand mk_temp(int id) {
    Operand op = mk_none();
    op.kind = A_TEMP;
    op.val = id;
    snprintf(op.name, sizeof(op.name), "t%d", id);
    return op;
}

Operand mk_label(int id) {
    Operand op = mk_none();
    op.kind = A_LABEL;
    op.val = id;
    snprintf(op.name, sizeof(op.name), "L%d", id);
    return op;
}

Operand mk_str(const char *str) {
    Operand op = mk_none();
    op.kind = A_STR;
    if (str) {
        strncpy(op.name, str, sizeof(op.name) - 1);
    }
    return op;
}

Quad new_quad(Op op, int relop, Operand a1, Operand a2, Operand res) {
    Quad q;
    q.op = op;
    q.relop = relop;
    q.a1 = a1;
    q.a2 = a2;
    q.res = res;
    return q;
}

char *new_temp(void) {
    static char buf[32];
    snprintf(buf, sizeof(buf), "t%d", temp_counter++);
    return buf;
}

char *new_label(void) {
    static char buf[32];
    snprintf(buf, sizeof(buf), "L%d", label_counter++);
    return buf;
}

void reset_counters(void) {
    temp_counter = 1;
    label_counter = 1;
}

void qlist_init(QList *list) {
    if (!list) return;
    list->cap = 16;
    list->n = 0;
    list->q = (Quad *)malloc(sizeof(Quad) * list->cap);
}

void qlist_add(QList *list, Quad quad) {
    if (!list) return;
    if (list->n >= list->cap) {
        list->cap *= 2;
        list->q = (Quad *)realloc(list->q, sizeof(Quad) * list->cap);
    }
    list->q[list->n++] = quad;
}

void qlist_free(QList *list) {
    if (!list) return;
    if (list->q) free(list->q);
    list->q = NULL;
    list->n = list->cap = 0;
}

int operand_eq(Operand a, Operand b) {
    if (a.kind != b.kind) return 0;
    switch (a.kind) {
        case A_NONE:
            return 1;
        case A_CONST:
            return a.val == b.val;
        case A_TEMP:
            return a.val == b.val || (strcmp(a.name, b.name) == 0);
        case A_LABEL:
        case A_VAR:
        case A_STR:
            return strcmp(a.name, b.name) == 0;
    }
    return 0;
}
