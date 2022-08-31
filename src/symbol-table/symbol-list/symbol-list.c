#include <stdio.h>
#include <string.h>
#include "symbol-list.h"

void add_symbol(list_t l, symbol_t* s) {
    add(l, s);
}

symbol_t* search_symbol_l(list_t l, char* name) {
    node_t* cur = (l.head)->next;
    symbol_t* s = NULL;
    while (cur != NULL) {
        if (!strcmp(name, ((symbol_t*) cur->value)->name)) {
            s = (symbol_t*) cur->value;
            break;
        }
        cur = cur->next;
    }
    return s;
}
