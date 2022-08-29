#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


list initList(void) {
    list* l = (list*) malloc(sizeof(list));
    l->head = (node*) malloc(sizeof(node));
    l->last = (node*) malloc(sizeof(node));
    return *l;
}

void add(list l, void* v) {
    node* n = (node*) malloc(sizeof(node));
    n->value = v;
    n->next = NULL;

    if (l.head->next == NULL) {
        l.head->next = n;
        l.last->next = n;
    } else {
        l.last->next->next = n;
        l.last->next = l.last->next->next;
    }
}
