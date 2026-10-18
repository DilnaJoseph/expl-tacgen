
#ifndef SYMTAB_H
#define SYMTAB_H

#include "ast.h"

typedef struct Symbol Symbol;

struct Symbol {
    char *name;
    DataType type;
    Symbol *next;
};

void symtab_init(void);
int symtab_add(const char *name, DataType type);
Symbol *symtab_lookup(const char *name);
void symtab_free(void);

#endif
