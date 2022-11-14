#include "asm-gen-utils.h"
#include <stdio.h>
#include <stdlib.h>

int has_offset(symbol_t* s) {
    return s->flag == BIN_OP_F || s->flag == UN_OP_F || s->flag == ID_F || s->flag == CALL_F || s->flag == PARAM_F;
}

int is_global_var(symbol_t* s) {
    return s->flag == ID_F && s->global;
}

char* get_asm_address(symbol_t* s) {
    char* address = (char*) malloc(50 * sizeof(char));
    if (s->flag == REG_F) {
        sprintf(address, "%%%s", s->name);
    } else if (s->flag == BASIC_F) {
        sprintf(address, "$%d", s->value);
    } else if (is_global_var(s)) {
        sprintf(address, "%s(%%rip)", s->name);
    } else if (has_offset(s)) {
        sprintf(address, "%d(%%rbp)", s->offset);
    } else {
        printf("Invalid type of address\n");
        exit(1);
    }
    return address;
}

int is_mem_ref(symbol_t* s) {
    return is_global_var(s) || has_offset(s);
}

char* get_edx_intermediation() {
    char* edx_interm = (char*) malloc(50 * sizeof(char));
    sprintf(edx_interm, ", %%edx");
    sprintf(edx_interm, "%s\n\tmovl    %%edx", edx_interm); 
    return edx_interm;
}

char* mov_operand(symbol_t* s, char* reg) {
    char* mov = (char*) malloc(100 * sizeof(char));
    sprintf(mov, "\tmovl    %s, %%%s", get_asm_address(s), reg);
    return mov;
}
