#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "flags.h"
#include "types.h"
#include "../list/list.h"

void addSymbol(list l, symbol s) {
    add(l, &s);
}

symbol searchSymbol(list l, char* name) {
    node* cur = (l.head)->next;
    symbol* s = (symbol*)malloc(sizeof(symbol));
    while (cur != NULL) {
        printf("%s\n",((symbol*) cur->value)->name);
        if (!strcmp(name, ((symbol*) cur->value)->name))
            s = (symbol*) cur->value;
        cur = cur->next;
    }
    return *s;
}
