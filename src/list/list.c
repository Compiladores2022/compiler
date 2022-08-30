#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

node* createNode(void * v) {
    node* n = (node*) malloc(sizeof(node));
    n->value = v;
    n->next = NULL;
    return n;
}

list initList(void) {
    list* l = (list*) malloc(sizeof(list));
    l->head = (node*) malloc(sizeof(node));
    l->last = (node*) malloc(sizeof(node));
    return *l;
}

void add(list l, void* v) {
    node* n = createNode(v);

    if (l.head->next == NULL) {
        l.head->next = n;
        l.last->next = n;
    } else {
        l.last->next->next = n;
        l.last->next = l.last->next->next;
    }
}

void addFirst(list l, void* v) {
    node* n = createNode(v);
    
    if (l.head->next == NULL) {
        l.head->next = n;
        l.last->next = n;
    } else {
        n->next = l.head->next;
        l.head->next = n;
    }
}

void* removeFirst(list l) {
    node* topNode = l.head->next;
    if (topNode) {
        l.head->next = l.head->next->next;
        return topNode->value;
    }
    return NULL;
}
