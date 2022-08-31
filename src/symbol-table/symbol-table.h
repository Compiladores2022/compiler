#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../symbol-list/symbol.h"
#include "../stack/stack.h"
#include "../list/list.h"

typedef struct {
    symstack_t* stack;
} symtable_t;

symtable_t init_symtable(void);

void push_level(symtable_t st);

void pop_level(symtable_t st);

void insert_symbol(symtable_t st, symbol_t* s);

symbol_t* search_symbol(symtable_t st, char* name);

#endif
