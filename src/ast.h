#ifndef AST_H
#define AST_H

typedef enum {
    EXPR_INT,
    EXPR_STRING,
    EXPR_VAR,
    EXPR_BINARY,
    EXPR_UNARY
} ExprKind;

typedef enum {
    AST_ADD, AST_SUB, AST_MUL, AST_DIV, AST_MOD,
    AST_NEG, AST_NOT,
    AST_LT, AST_GT, AST_LE, AST_GE, AST_EQ, AST_NE,
    AST_AND, AST_OR
} ExprOp;

typedef struct Expr Expr;

struct Expr {
    ExprKind kind;

    union {
        long int_value;
        char *string_value;
        char *var_name;

        struct {
            ExprOp op;
            Expr *left;
            Expr *right;
        } binary;

        struct {
            ExprOp op;
            Expr *operand;
        } unary;
    } data;
};

typedef enum {
    STMT_ASSIGN,
    STMT_READ,
    STMT_WRITE,
    STMT_IF,
    STMT_WHILE,
    STMT_BREAK,
    STMT_CONTINUE,
    STMT_RETURN
} StmtKind;

typedef struct Stmt Stmt;

struct Stmt {
    StmtKind kind;
    Stmt *next;

    union {
        struct {
            char *name;
            Expr *value;
        } assign;

        char *read_name;

        Expr *write_expr;

        struct {
            Expr *condition;
            Stmt *then_body;
            Stmt *else_body;
        } if_stmt;

        struct {
            Expr *condition;
            Stmt *body;
        } while_stmt;

        Expr *return_expr;
    } data;
};

typedef enum {
    TYPE_INT,
    TYPE_STR
} DataType;

typedef struct Decl Decl;

struct Decl {
    char *name;
    DataType type;
    Decl *next;
};

typedef struct {
    Decl *decls;
    Stmt *statements;
} Program;
Expr *make_int(long value);
Expr *make_string(const char *value);
Expr *make_var(const char *name);
Expr *make_binary(ExprOp op, Expr *left, Expr *right);
Expr *make_unary(ExprOp op, Expr *operand);

Stmt *make_assign(const char *name, Expr *value);
Stmt *make_read(const char *name);
Stmt *make_write(Expr *value);
Stmt *make_if(Expr *condition, Stmt *then_body, Stmt *else_body);
Stmt *make_while(Expr *condition, Stmt *body);
Stmt *make_break(void);
Stmt *make_continue(void);
Stmt *make_return(Expr *value);

Decl *make_decl(const char *name, DataType type);
Program *make_program(Decl *decls, Stmt *statements);

Stmt *append_stmt(Stmt *list, Stmt *stmt);
Decl *append_decl(Decl *list, Decl *decl);
#endif