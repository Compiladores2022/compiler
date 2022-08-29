#include <stdlib.h>
#include <stdio.h>
#include "list.h"


list* initList(void) {
    list* l = (list*) malloc(sizeof(list));
    l->head = (node*) malloc(sizeof(node));
    return l;
}

void add(list l, void* v) {
    node* n = (node*) malloc(sizeof(node));
    n->value = v;
    n->next = NULL;

    node* cursor = l.head;
    while (cursor->next != NULL) {
        cursor = cursor->next;
    }
    cursor->next = n;
}
