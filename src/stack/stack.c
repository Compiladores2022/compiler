#include <stdio.h>
#include "stack.h"
#include "../list/list.h"

symstack_t init_stack(void) {
    return init_list();
}

void push(symstack_t s, void* v) {
    add_first(s, v);
}

void* pop(symstack_t s) {
    return remove_first(s);
}

void* top(symstack_t s) {
    node_t* top_node = s.head->next;
    if (top_node) {
        return top_node->value;
    }
    return NULL;
}
