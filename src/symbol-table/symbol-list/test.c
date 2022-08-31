#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../symbol/symbol.h"
#include "../symbol/flags.h"
#include "../symbol/types.h"
#include "../../list/list.h"
#include "symbol-list.h"

int main() {

    list_t l = init_list();
    symbol_t* symbols[10];
    char* name;

    for (int i = 0; i < 10; i++) {
        int* x = (int*) malloc(sizeof(int));
        *x = i;
        name = malloc(15*sizeof(char));
        snprintf(name, 15, "test_%d", *x);
        symbols[i] = create_symbol();
        set_name(symbols[i], name);
    }

    for (int i = 0; i < 10; i++) {
        add_symbol(l, symbols[i]);
    }

    symbol_t* t = search_symbol_l(l, "test_5");
    printf("Symbol found: %s\n", t->name);

    printf("Iterating list: \n");
    node_t* cursor = (l.head)->next;
    while (cursor != NULL) {
        symbol_t v = *(symbol_t*) cursor->value;
        printf("%s \n", v.name);
        cursor = cursor->next;
    }

    return 0;
}
