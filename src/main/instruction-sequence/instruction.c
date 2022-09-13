#include <stdlib.h>
#include "instruction.h"

instruction_t* create_instruction(inst_type_t type, symbol_t* s1, symbol_t* s2, symbol_t* s3) {
    instruction_t* instruction = malloc(sizeof(instruction_t));
    instruction->type = type;
    instruction->s1 = s1;
    instruction->s2 = s2;
    instruction->s3 = s3;
    return instruction;
}
