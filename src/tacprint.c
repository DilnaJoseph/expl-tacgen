#include "tacprint.h"
#include <string.h>

static const char *relop_str(int relop) {
    switch (relop) {
        case REL_LT: return "<";
        case REL_GT: return ">";
        case REL_LE: return "<=";
        case REL_GE: return ">=";
        case REL_EQ: return "==";
        case REL_NE: return "!=";
        default:     return "==";
    }
}

static void print_operand(FILE *fp, const Operand *op) {
    if (!op || op->kind == A_NONE) return;
    if (op->kind == A_STR) {
        if (op->name[0] == '"') {
            fprintf(fp, "%s", op->name);
        } else {
            fprintf(fp, "\"%s\"", op->name);
        }
    } else {
        fprintf(fp, "%s", op->name);
    }
}

void print_quad(FILE *fp, const Quad *q) {
    if (!fp || !q) return;

    switch (q->op) {
        case OP_ADD:
            print_operand(fp, &q->res);
            fprintf(fp, " = ");
            print_operand(fp, &q->a1);
            fprintf(fp, " + ");
            print_operand(fp, &q->a2);
            fprintf(fp, "\n");
            break;
        case OP_SUB:
            print_operand(fp, &q->res);
            fprintf(fp, " = ");
            print_operand(fp, &q->a1);
            fprintf(fp, " - ");
            print_operand(fp, &q->a2);
            fprintf(fp, "\n");
            break;
        case OP_MUL:
            print_operand(fp, &q->res);
            fprintf(fp, " = ");
            print_operand(fp, &q->a1);
            fprintf(fp, " * ");
            print_operand(fp, &q->a2);
            fprintf(fp, "\n");
            break;
        case OP_DIV:
            print_operand(fp, &q->res);
            fprintf(fp, " = ");
            print_operand(fp, &q->a1);
            fprintf(fp, " / ");
            print_operand(fp, &q->a2);
            fprintf(fp, "\n");
            break;
        case OP_MOD:
            print_operand(fp, &q->res);
            fprintf(fp, " = ");
            print_operand(fp, &q->a1);
            fprintf(fp, " %% ");
            print_operand(fp, &q->a2);
            fprintf(fp, "\n");
            break;
        case OP_COPY:
            print_operand(fp, &q->res);
            fprintf(fp, " = ");
            print_operand(fp, &q->a1);
            fprintf(fp, "\n");
            break;
        case OP_NEG:
            print_operand(fp, &q->res);
            fprintf(fp, " = - ");
            print_operand(fp, &q->a1);
            fprintf(fp, "\n");
            break;
        case OP_LABEL:
            print_operand(fp, &q->res);
            fprintf(fp, ":\n");
            break;
        case OP_GOTO:
            fprintf(fp, "goto ");
            print_operand(fp, &q->res);
            fprintf(fp, "\n");
            break;
        case OP_IFREL:
            fprintf(fp, "if ");
            print_operand(fp, &q->a1);
            fprintf(fp, " %s ", relop_str(q->relop));
            print_operand(fp, &q->a2);
            fprintf(fp, " goto ");
            print_operand(fp, &q->res);
            fprintf(fp, "\n");
            break;
        case OP_READ:
            fprintf(fp, "read ");
            print_operand(fp, &q->res);
            fprintf(fp, "\n");
            break;
        case OP_WRITE:
            fprintf(fp, "write ");
            print_operand(fp, &q->a1);
            fprintf(fp, "\n");
            break;
    }
}

void print_quads(FILE *fp, const Quad *q, int n) {
    if (!fp || !q) return;
    for (int i = 0; i < n; i++) {
        print_quad(fp, &q[i]);
    }
}
