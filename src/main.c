#include <stdio.h>
#include "list/list.h"
#include "symbols-table/symbol.h"

int main() {
    list l = initList();
    symbol s;
    printf("Hi!\n");
    addSymbol(l, s);
    return 0;
}
