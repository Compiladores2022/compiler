#ifndef STACK_H
#define STACK_H 

#include "../list/list.h"

typedef list_t stack_t;

stack_t init_stack(void);

void push(stack_t s, void* v);

void* pop(stack_t s);

void* top(stack_t s);

#endif 
