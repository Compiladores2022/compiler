#ifndef SYMBOL_H
#define SYMBOL_H

#include "flags.h"
#include "types.h"
#include "../list/list.h"

typedef struct {
    flag_t flag;
    type_t type;
    int value;
    char* name;
    int offset;
    int lineno;
    list_t* params;
    int global;
} symbol_t;

symbol_t* create_symbol(void);

void set_flag(symbol_t* s, flag_t flag);

void set_type(symbol_t* s, type_t flag);

void set_value(symbol_t* s, int value);

void set_name(symbol_t* s, char* value);

#endif
