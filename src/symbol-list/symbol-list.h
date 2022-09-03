#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H

#include "../libs/list/list.h"
#include "../symbol/symbol.h"

void add_symbol(list_t* l, symbol_t* s);

symbol_t* search_symbol_l(list_t* l, char* name);

#endif
