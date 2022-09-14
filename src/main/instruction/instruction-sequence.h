#ifndef INSTRUCTION_SEQ_H
#define INSTRUCTION_SEQ_H

#include "instruction.h"
#include "../list/list.h"

list_t* new_instruction_seq();

void add_instruction(list_t* instruction_seq, instruction_t* instruction);

#endif
