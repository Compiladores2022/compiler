#include <stdio.h>
#include <string.h>
#include "symbol-table.h"

int main() {
    symtable_t* st = init_symtable();
    push_level(st);

    symbol_t* s1 = create_symbol();
    set_name(s1, "test1");
    printf("Created s1\n");
    symbol_t* s2 = create_symbol();
    set_name(s2, "test2");
    printf("Created s2\n");
    symbol_t* s3 = create_symbol();
    set_name(s3, "test3");
    printf("Created s3\n");
    symbol_t* s4 = create_symbol();
    set_name(s4, "test4");
    printf("Created s4\n");
    symbol_t* s5 = create_symbol();
    set_name(s5, "test5");
    printf("Created s5\n");

    /*
    symstack_t* stack = init_stack();
    list_t* list = init_list();

    add_symbol(list, s1);
    push(stack, list);

    list_t* l_ptr = (list_t*) top(stack);
    symbol_t* s = (symbol_t*) (l_ptr->head)->next->value;
    printf("%s", s->name);
    */

    insert_symbol(st, s1);
    insert_symbol(st, s2);
    push_level(st);
    insert_symbol(st, s3);
    insert_symbol(st, s4);
    push_level(st);
    insert_symbol(st, s5);

    symbol_t* s = search_symbol(st, "test3");
    printf("Symbol found: %s\n", s->name);
    

    return 0;
}
