#include "instruction.h"
#include "../list/list.h"

list_t* new_instruction_seq() {
    return init_list();
}

void add_instruction(list_t* instruction_seq, instruction_t* instruction) {
    add(instruction_seq, instruction);
}
