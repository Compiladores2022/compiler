#include <stdio.h>
#include <stdlib.h>
#include "asm-gen.h"
#include "../instruction/instruction.h"

char* create_mov_instruction(instruction_t* instruction) {
    return NULL;
}

char* create_add_instruction(instruction_t* instruction) {
    return NULL;
}

char* create_sub_instruction(instruction_t* instruction) {
    return NULL;
}

char* create_mul_instruction(instruction_t* instruction) {
    return NULL;
}

char* create_and_instruction(instruction_t* instruction) {
    return NULL;
}

char* create_or_instruction(instruction_t* instruction) {
    return NULL;
}

char* create_ret_instruction(instruction_t* instruction) {
    return NULL;
}

char* create_asm_instruction(instruction_t* instruction) {
    switch (instruction->type) {
        case MOV:
            return create_mov_instruction(instruction);
        case ADD:
            return create_add_instruction(instruction);
        case SUB:
            return create_sub_instruction(instruction);
        case MUL:
            return create_mul_instruction(instruction);
        case AND:
            return create_and_instruction(instruction);
        case OR:
            return create_or_instruction(instruction);
        case RET:
            return create_ret_instruction(instruction);
        default:
            printf("Invalid instruction type\n");
            exit(1);
    }
}
