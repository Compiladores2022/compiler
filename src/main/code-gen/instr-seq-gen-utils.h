#ifndef INSTR_SEQ_GEN_UTILS
#define INSTR_SEQ_GEN_UTILS

#include "../list/list.h"
#include "../tree/tree-node.h"
#include "../instruction/instruction.h"

symbol_t* get_label();
        
symbol_t* create_register(char* reg_name);

instr_type_t bin_op_to_instr_type(char* op);

instr_type_t un_op_to_instr_type(char* op);

void enlist_vars_declaration(tree_node_t* root, list_t* list);

char* type_to_str(instr_type_t type);

void show_instruction_seq(list_t* instructions);

#endif
