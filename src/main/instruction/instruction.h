#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../symbol/symbol.h"

typedef enum inst_type_t {
    MOV,
    ADD,
    SUB,
    MUL,
    AND,
    OR,
    RET
} inst_type_t;

typedef struct instruction_t {
    inst_type_t type;
    symbol_t* s1;
    symbol_t* s2;
    symbol_t* s3;
} instruction_t;

instruction_t* new_instruction(inst_type_t type, symbol_t* s1, symbol_t* s2, symbol_t* s3);

#endif
