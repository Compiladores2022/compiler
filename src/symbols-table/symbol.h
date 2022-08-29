#ifndef SYMBOL_H
#define SYMBOL_H

#include "flags.h"
#include "types.h"
#include "../list/list.h"

typedef struct {
    s_flag flag;
    s_type type;
    int value;
    char* name;
} symbol;

void addSymbol(list l, symbol s);

symbol searchSymbol(list l, char* name);

#endif
