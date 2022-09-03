#include <stdio.h>
#include <stdlib.h>

#include "symbol-table.h"
#include "../symbol-list/symbol-list.h"

symtable_t* init_symtable(void) {
    symtable_t* st = (symtable_t*) malloc(sizeof(symtable_t));
    symstack_t* stack = init_stack();
    st->stack = stack;
    return st;
}

void push_level(symtable_t* st) {
    list_t* l = init_list();
    push(st->stack, l);
}

void pop_level(symtable_t* st) {
    pop(st->stack);
}

void insert_symbol(symtable_t* st, symbol_t* s) {
    list_t* l = (list_t*) top(st->stack);
    add(l, s);
}

symbol_t* search_symbol(symtable_t* st, char* name) {
    node_t* cursor = st->stack->head->next;
    while (cursor) {
        list_t* l = (list_t*) cursor->value;
        symbol_t* s = search_symbol_l(l, name);
        if (s) {
            return s;
        }
        cursor = cursor->next;
    }
    return NULL;
}
