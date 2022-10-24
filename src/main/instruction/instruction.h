#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "../symbol/symbol.h"

typedef enum instr_type_t {
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    AND,
    OR,
    EQ,
    GT,
    LT,
    MIN,
    NEG,
    RET,
    JMP,
    LBL
} instr_type_t;

typedef struct instruction_t {
    instr_type_t type;
    symbol_t* s1;
    symbol_t* s2;
    symbol_t* s3;
} instruction_t;

instruction_t* new_instruction(instr_type_t type, symbol_t* s1, symbol_t* s2, symbol_t* s3);

#endif
