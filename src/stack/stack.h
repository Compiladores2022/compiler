#ifndef STACK_H
#define STACK_H 

#include "../list/list.h"

typedef list stack;

stack initStack(void);

void push(stack s, void* v);

void* pop(stack s);

void* top(stack s);

#endif 
