#include "tacread.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static Operand parse_operand(const char *tok) {
    if (!tok || strlen(tok) == 0) return mk_none();

    if (tok[0] == '"') {
        return mk_str(tok);
    }
    char *endptr;
    long val = strtol(tok, &endptr, 10);
    if (*endptr == '\0') {
        return mk_const(val);
    }
    if (tok[0] == 't' && isdigit(tok[1])) {
        long tid = strtol(tok + 1, &endptr, 10);
        if (*endptr == '\0') {
            return mk_temp((int)tid);
        }
    }
    if (tok[0] == 'L' && isdigit(tok[1])) {
        long lid = strtol(tok + 1, &endptr, 10);
        if (*endptr == '\0') {
            return mk_label((int)lid);
        }
    }
    return mk_var(tok);
}

static RelOp parse_relop(const char *s) {
    if (strcmp(s, "<") == 0) return REL_LT;
    if (strcmp(s, ">") == 0) return REL_GT;
    if (strcmp(s, "<=") == 0) return REL_LE;
    if (strcmp(s, ">=") == 0) return REL_GE;
    if (strcmp(s, "==") == 0) return REL_EQ;
    if (strcmp(s, "!=") == 0) return REL_NE;
    return REL_NONE;
}

Quad *read_tac(FILE *fp, int *n) {
    if (!fp || !n) return NULL;
    QList list;
    qlist_init(&list);

    char line[512];
    int lineno = 0;

    while (fgets(line, sizeof(line), fp)) {
        lineno++;
        char *p = line;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0' || *p == '#') continue;

        char *end = p + strlen(p) - 1;
        while (end >= p && isspace((unsigned char)*end)) {
            *end = '\0';
            end--;
        }

        /* 1. Label: "L1:" */
        if (p[strlen(p) - 1] == ':') {
            p[strlen(p) - 1] = '\0';
            Operand res = parse_operand(p);
            qlist_add(&list, new_quad(OP_LABEL, REL_NONE, mk_none(), mk_none(), res));
            continue;
        }

        /* 2. goto L1 */
        if (strncmp(p, "goto ", 5) == 0) {
            char target[64];
            if (sscanf(p + 5, "%63s", target) == 1) {
                Operand res = parse_operand(target);
                qlist_add(&list, new_quad(OP_GOTO, REL_NONE, mk_none(), mk_none(), res));
                continue;
            }
        }

        /* 3. read x */
        if (strncmp(p, "read ", 5) == 0) {
            char target[64];
            if (sscanf(p + 5, "%63s", target) == 1) {
                Operand res = parse_operand(target);
                qlist_add(&list, new_quad(OP_READ, REL_NONE, mk_none(), mk_none(), res));
                continue;
            }
        }

        /* 4. write x */
        if (strncmp(p, "write ", 6) == 0) {
            char *target = p + 6;
            while (isspace((unsigned char)*target)) target++;
            Operand a1 = parse_operand(target);
            qlist_add(&list, new_quad(OP_WRITE, REL_NONE, a1, mk_none(), mk_none()));
            continue;
        }

        /* 5. if a1 relop a2 goto L1 */
        if (strncmp(p, "if ", 3) == 0) {
            char a1_s[64], rel_s[16], a2_s[64], goto_s[16], res_s[64];
            if (sscanf(p + 3, "%63s %15s %63s %15s %63s", a1_s, rel_s, a2_s, goto_s, res_s) == 5) {
                Operand a1 = parse_operand(a1_s);
                Operand a2 = parse_operand(a2_s);
                Operand res = parse_operand(res_s);
                RelOp rop = parse_relop(rel_s);
                qlist_add(&list, new_quad(OP_IFREL, rop, a1, a2, res));
                continue;
            }
        }

        /* 6. Assignments */
        char res_s[64], eq_s[16], rest[256];
        if (sscanf(p, "%63s %15s %[^\n]", res_s, eq_s, rest) >= 3 && strcmp(eq_s, "=") == 0) {
            Operand res = parse_operand(res_s);
            char *r = rest;
            while (isspace((unsigned char)*r)) r++;

            if (r[0] == '-' && isspace((unsigned char)r[1])) {
                char a1_s[64];
                if (sscanf(r + 1, "%63s", a1_s) == 1) {
                    Operand a1 = parse_operand(a1_s);
                    qlist_add(&list, new_quad(OP_NEG, REL_NONE, a1, mk_none(), res));
                    continue;
                }
            }

            char a1_s[64], op_s[16], a2_s[64];
            if (sscanf(r, "%63s %15s %63s", a1_s, op_s, a2_s) == 3) {
                Operand a1 = parse_operand(a1_s);
                Operand a2 = parse_operand(a2_s);
                Op bin_op = OP_ADD;
                if (strcmp(op_s, "+") == 0) bin_op = OP_ADD;
                else if (strcmp(op_s, "-") == 0) bin_op = OP_SUB;
                else if (strcmp(op_s, "*") == 0) bin_op = OP_MUL;
                else if (strcmp(op_s, "/") == 0) bin_op = OP_DIV;
                else if (strcmp(op_s, "%") == 0) bin_op = OP_MOD;

                qlist_add(&list, new_quad(bin_op, REL_NONE, a1, a2, res));
                continue;
            }

            /* Single copy: res = a1 */
            Operand a1 = parse_operand(r);
            qlist_add(&list, new_quad(OP_COPY, REL_NONE, a1, mk_none(), res));
            continue;
        }

        fprintf(stderr, "Syntax error parsing TAC at line %d: %s\n", lineno, line);
    }

    *n = list.n;
    return list.q;
}
