#ifndef LIST_H
#define LIST_H 

typedef struct node {
    void* value;
    struct node* next;
} node;

typedef struct {
    node* head;
    node* last;
} list;

list initList(void);

void add(list l, void* v);

#endif 
