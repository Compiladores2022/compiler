#ifndef ASM_GEN
#define ASM_GEN

#include "../instruction/instruction.h"
#include "../list/list.h"

void create_asm(char* filename, list_t* instruction_seq);

#endif
