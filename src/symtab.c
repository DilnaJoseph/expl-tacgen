#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"
static Symbol *symbol_head = NULL;

static char *copy_name(const char *name)
{
    char *copy = malloc(strlen(name) + 1);

    if (copy == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    strcpy(copy, name);
    return copy;
}

void symtab_init(void)
{
    symtab_free();
}

Symbol *symtab_lookup(const char *name)
{
    Symbol *current = symbol_head;

    while (current != NULL) {
        if (strcmp(current->name, name) == 0)
            return current;

        current = current->next;
    }

    return NULL;
}

int symtab_add(const char *name, DataType type)
{
    
    if (symtab_lookup(name) != NULL)
        return 0;

    Symbol *symbol = malloc(sizeof(Symbol));

    if (symbol == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    symbol->name = copy_name(name);
    symbol->type = type;

    symbol->next = symbol_head;
    symbol_head = symbol;

    return 1;
}

void symtab_free(void)
{
    Symbol *current = symbol_head;

    while (current != NULL) {
        Symbol *next = current->next;

        free(current->name);
        free(current);

        current = next;
    }

    symbol_head = NULL;
}
