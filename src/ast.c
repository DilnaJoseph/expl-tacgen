
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
static char *copy_string(const char *s)
{
    if (s == NULL)
        return NULL;

    char *copy = malloc(strlen(s) + 1);

    if (copy == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, s);
    return copy;
}

Expr *make_int(long value)
{
    Expr *e = malloc(sizeof(Expr));

    if (e == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    e->kind = EXPR_INT;
    e->data.int_value = value;
    return e;
}

Expr *make_string(const char *value)
{
    Expr *e = malloc(sizeof(Expr));

    if (e == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    e->kind = EXPR_STRING;
    e->data.string_value = copy_string(value);
    return e;
}

Expr *make_var(const char *name)
{
    Expr *e = malloc(sizeof(Expr));

    if (e == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    e->kind = EXPR_VAR;
    e->data.var_name = copy_string(name);
    return e;
}

Expr *make_binary(ExprOp op, Expr *left, Expr *right)
{
    Expr *e = malloc(sizeof(Expr));

    if (e == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    e->kind = EXPR_BINARY;
    e->data.binary.op = op;
    e->data.binary.left = left;
    e->data.binary.right = right;
    return e;
}

Expr *make_unary(ExprOp op, Expr *operand)
{
    Expr *e = malloc(sizeof(Expr));

    if (e == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    e->kind = EXPR_UNARY;
    e->data.unary.op = op;
    e->data.unary.operand = operand;
    return e;
}


Stmt *make_assign(const char *name, Expr *value)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_ASSIGN;
    s->data.assign.name = copy_string(name);
    s->data.assign.value = value;
    return s;
}

Stmt *make_read(const char *name)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_READ;
    s->data.read_name = copy_string(name);
    return s;
}

Stmt *make_write(Expr *value)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_WRITE;
    s->data.write_expr = value;
    return s;
}

Stmt *make_if(Expr *condition, Stmt *then_body,
              Stmt *else_body)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_IF;
    s->data.if_stmt.condition = condition;
    s->data.if_stmt.then_body = then_body;
    s->data.if_stmt.else_body = else_body;
    return s;
}

Stmt *make_while(Expr *condition, Stmt *body)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_WHILE;
    s->data.while_stmt.condition = condition;
    s->data.while_stmt.body = body;
    return s;
}

Stmt *make_break(void)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_BREAK;
    return s;
}

Stmt *make_continue(void)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_CONTINUE;
    return s;
}

Stmt *make_return(Expr *value)
{
    Stmt *s = calloc(1, sizeof(Stmt));

    if (s == NULL) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }

    s->kind = STMT_RETURN;
    s->data.return_expr = value;
    return s;
}

Decl *make_decl(const char *name, DataType type)
{
    Decl *d = malloc(sizeof(Decl));

    if (d == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    d->name = copy_string(name);
    d->type = type;
    d->next = NULL;
    return d;
}

Program *make_program(Decl *decls, Stmt *statements)
{
    Program *p = malloc(sizeof(Program));

    if (p == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    p->decls = decls;
    p->statements = statements;
    return p;
}

Stmt *append_stmt(Stmt *list, Stmt *stmt)
{
    if (list == NULL)
        return stmt;

    Stmt *current = list;

    while (current->next != NULL)
        current = current->next;

    current->next = stmt;
    return list;
}

Decl *append_decl(Decl *list, Decl *decl)
{
    if (list == NULL)
        return decl;

    Decl *current = list;

    while (current->next != NULL)
        current = current->next;

    current->next = decl;
    return list;
}
