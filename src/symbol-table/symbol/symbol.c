#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "flags.h"
#include "types.h"
#include "../../list/list.h"

symbol_t* create_symbol(void) {
    symbol_t* s = malloc(sizeof(symbol_t));
    return s;
}

void set_flag(symbol_t* s, flag_t flag) {
    s->flag = flag;
}

void set_type(symbol_t* s, type_t type) {
    s->type = type;
}

void set_value(symbol_t* s, int value) {
    s->value = value;
}

void set_name(symbol_t* s, char* name) {
    s->name = name;
}
