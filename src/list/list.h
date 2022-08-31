#ifndef LIST_H
#define LIST_H 

typedef struct node_t {
    void* value;
    struct node_t* next;
} node_t;

typedef struct {
    node_t* head;
    node_t* last;
} list_t;

list_t init_list(void);

void add(list_t l, void* v);

void add_first(list_t l, void* v);

void* remove_first(list_t l);

#endif 
