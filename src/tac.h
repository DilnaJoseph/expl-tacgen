#ifndef TAC_H
#define TAC_H

#include <stdio.h>

/* Operator types supported in the TAC representation */
typedef enum {
    OP_ADD,     /* res = a1 + a2 */
    OP_SUB,     /* res = a1 - a2 */
    OP_MUL,     /* res = a1 * a2 */
    OP_DIV,     /* res = a1 / a2 */
    OP_MOD,     /* res = a1 % a2 */
    OP_COPY,    /* res = a1 */
    OP_NEG,     /* res = - a1 */
    OP_LABEL,   /* res: (e.g. L1:) */
    OP_GOTO,    /* goto res */
    OP_IFREL,   /* if a1 relop a2 goto res */
    OP_READ,    /* read res */
    OP_WRITE    /* write a1 */
} Op;

/* Relational operator codes */
typedef enum {
    REL_NONE = 0,
    REL_LT,     /* <  */
    REL_GT,     /* >  */
    REL_LE,     /* <= */
    REL_GE,     /* >= */
    REL_EQ,     /* == */
    REL_NE      /* != */
} RelOp;

/* Operand categories */
typedef enum {
    A_NONE = 0,
    A_VAR,      /* User variable */
    A_TEMP,     /* Temporary variable (t1, t2, ...) */
    A_CONST,    /* Long integer constant */
    A_STR,      /* String literal */
    A_LABEL     /* Target label (L1, L2, ...) */
} AKind;

/* TAC Operand descriptor */
typedef struct {
    AKind kind;
    long val;           /* Used when kind == A_CONST or temp/label IDs */
    char name[64];      /* Identifier name or string text */
} Operand;

/* Three-Address Code Quadruple */
typedef struct {
    Op op;
    int relop;          /* RelOp enum value if OP_IFREL, else REL_NONE */
    Operand a1;
    Operand a2;
    Operand res;
} Quad;

/* Growable list of quads */
typedef struct {
    Quad *q;
    int n;
    int cap;
} QList;

/* Basic Block abstraction */
typedef struct {
    int id;
    Quad *q;
    int n;
} Block;

/* Global statistics tracker */
typedef struct {
    int folds;
    int cse;
    int quads_before;
    int quads_after;
    int blocks;
} Stats;

extern Stats stats;

/* Helper constructors */
Operand mk_none(void);
Operand mk_const(long val);
Operand mk_var(const char *name);
Operand mk_temp(int id);
Operand mk_label(int id);
Operand mk_str(const char *str);

Quad new_quad(Op op, int relop, Operand a1, Operand a2, Operand res);

char *new_temp(void);
char *new_label(void);
void reset_counters(void);

void qlist_init(QList *list);
void qlist_add(QList *list, Quad quad);
void qlist_free(QList *list);

int operand_eq(Operand a, Operand b);

#endif /* TAC_H */
