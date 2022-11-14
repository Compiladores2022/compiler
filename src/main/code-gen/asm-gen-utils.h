#ifndef ASM_GEN_UTILS
#define ASM_GEN_UTILS

#include "../symbol/symbol.h"

int has_offset(symbol_t* s);

int is_global_var(symbol_t* s);

char* get_asm_address(symbol_t* s);

int is_mem_ref(symbol_t* s);

char* get_edx_intermediation();

char* mov_operand(symbol_t* s, char* reg);

#endif
