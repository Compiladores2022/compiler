#ifndef STACK_H
#define STACK_H 

#include "../list/list.h"

typedef list_t symstack_t;

symstack_t* init_stack(void);

void push(symstack_t* s, void* v);

void* pop(symstack_t* s);

void* top(symstack_t* s);

#endif 
