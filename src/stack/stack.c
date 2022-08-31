#include <stdio.h>
#include "stack.h"
#include "../list/list.h"

stack_t init_stack(void) {
    return init_list();
}

void push(stack_t s, void* v) {
    add_first(s, v);
}

void* pop(stack_t s) {
    return remove_first(s);
}

void* top(stack_t s) {
    node_t* top_node = s.head->next;
    if (top_node) {
        return top_node->value;
    }
    return NULL;
}
