#ifndef SYMBOL_H
#define SYMBOL_H

#include "flags.h"
#include "types.h"

typedef struct {
    s_flag flag;
    s_type type;
    int value;
    char* name;
} symbol;

#endif
