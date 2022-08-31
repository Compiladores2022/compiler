#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

node_t* create_node(void * v) {
    node_t* n = (node_t*) malloc(sizeof(node_t));
    n->value = v;
    n->next = NULL;
    return n;
}

list_t init_list(void) {
    list_t* l = (list_t*) malloc(sizeof(list_t));
    l->head = (node_t*) malloc(sizeof(node_t));
    l->last = (node_t*) malloc(sizeof(node_t));
    return *l;
}

void add(list_t l, void* v) {
    node_t* n = create_node(v);

    if (l.head->next == NULL) {
        l.head->next = n;
        l.last->next = n;
    } else {
        l.last->next->next = n;
        l.last->next = l.last->next->next;
    }
}

void add_first(list_t l, void* v) {
    node_t* n = create_node(v);
    
    if (l.head->next == NULL) {
        l.head->next = n;
        l.last->next = n;
    } else {
        n->next = l.head->next;
        l.head->next = n;
    }
}

void* remove_first(list_t l) {
    node_t* top_node = l.head->next;
    if (top_node) {
        l.head->next = l.head->next->next;
        return top_node->value;
    }
    return NULL;
}
