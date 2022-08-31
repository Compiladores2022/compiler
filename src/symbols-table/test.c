#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "flags.h"
#include "types.h"
#include "../list/list.h"

int main() {

    list l = initList();
    symbol s1 = {VAR, INT, 0, "test1"};
    symbol s2 = {VAR, INT, 0, "test2"};

    printf("Adding %s\n", s1.name);
    addSymbol(l, s1);
    printf("Adding %s\n", s2.name);
    addSymbol(l, s2);

    symbol t = searchSymbol(l, "test2");
    printf("Symbol found: %s\n", t.name);

    printf("Iterating list: \n");
    printf("Here 1\n");
    node* cursor = (l.head)->next;
    printf("Here 2\n");
    while (cursor != NULL) {
    printf("Here 3\n");
        symbol v = *(symbol *) cursor->value;
        printf("Here 4\n");
        printf("%s \n", v.name);
        printf("Here 5\n");
        cursor = cursor->next;
        printf("Here 6\n");
    }

    return 0;
}
