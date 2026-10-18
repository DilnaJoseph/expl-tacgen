#include "tac.h"
#include <stdlib.h>

/* Temporary stub until Kavitha's parser and code generator land */
Quad *gen_tac(void *ast_root, int *n) {
    (void)ast_root;
    QList list;
    qlist_init(&list);

    /* Generates:
       t = 2 + 3
       x = t * 4
       write x
    */
    qlist_add(&list, new_quad(OP_ADD, REL_NONE, mk_const(2), mk_const(3), mk_var("t")));
    qlist_add(&list, new_quad(OP_MUL, REL_NONE, mk_var("t"), mk_const(4), mk_var("x")));
    qlist_add(&list, new_quad(OP_WRITE, REL_NONE, mk_var("x"), mk_none(), mk_none()));

    *n = list.n;
    return list.q;
}
