#include <stdio.h>
#include "stack.h"
#include "../list/list.h"

stack initStack(void) {
    return initList();
}

void push(stack s, void* v) {
    addFirst(s, v);
}

void* pop(stack s) {
    return removeFirst(s);
}

void* top(stack s) {
    node* topNode = s.head->next;
    if (topNode) {
        return topNode->value;
    }
    return NULL;
}
