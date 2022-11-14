#include <stdlib.h>
#include <string.h>
#include "instr-seq-gen-utils.h"
#include "../utils/utils.h"
#include "../list/list.h"
#include "../tree/tree-node.h"
#include "../instruction/instruction.h"
#include "../instruction/instruction-sequence.h"

list_t* instruction_seq;

char* regs_names[6] = {"edi", "esi", "edx","ecx", "r8d", "r9d"};

symbol_t* curr_proc_out_lbl;

tree_node_t* global_decl_node;

void build_instruction_seq(symbol_t* s, tree_node_t* node) {
    if (s->flag == BIN_OP_F) {
        symbol_t* left = (symbol_t*) node->left->value;
        symbol_t* right = (symbol_t*) node->right->value;
        instr_type_t type = bin_op_to_instr_type(s->name);
        instruction_t* instruction = new_instruction(type, left, right, s);
        add_instruction(instruction_seq, instruction);
    }
    if (s->flag == UN_OP_F) {
        symbol_t* middle = (symbol_t*) node->middle->value;
        instr_type_t type = un_op_to_instr_type(s->name);
        instruction_t* instruction = new_instruction(type, middle, NULL, s);
        add_instruction(instruction_seq, instruction);
    }
    if (s->flag == ASSIGN_F || s->flag == DECL_F) {
        symbol_t* left = (symbol_t*) node->left->value;
        symbol_t* right = (symbol_t*) node->right->value;
        instruction_t* instruction = new_instruction(MOV, right, NULL, left);
        add_instruction(instruction_seq, instruction);
    }
    if (s->flag == RETURN_F) {
        if (node->middle) {
            symbol_t* middle = (symbol_t*) node->middle->value;
            instruction_t* instruction = new_instruction(RET, curr_proc_out_lbl, NULL, middle);
            add_instruction(instruction_seq, instruction);
        } else {
            instruction_t* instruction = new_instruction(RET, curr_proc_out_lbl, NULL, NULL);
            add_instruction(instruction_seq, instruction);
        }
    }
    if (s->flag == IF_F) {
        // generate Condition instructions
        traverse_tree(node->middle, build_instruction_seq, 1);
        symbol_t* condition = (symbol_t*) node->middle->value;
        symbol_t* skip_then_label = get_label();
        symbol_t* skip_else_label = get_label();
        // JMP !Condition Label1 (skip then)
        instruction_t* instruction = new_instruction(JE, condition, NULL, skip_then_label);
        add_instruction(instruction_seq, instruction);
        // generate THEN Block instructions
        traverse_tree(node->left, build_instruction_seq, 1);
        if (node->right) {
            // JMP Label2 (skip else)
            instruction = new_instruction(JMP, NULL, NULL, skip_else_label);
            add_instruction(instruction_seq, instruction);
        }
        // Label1 (skip then)
        instruction = new_instruction(LBL, NULL, NULL, skip_then_label);
        add_instruction(instruction_seq, instruction);
        if (node->right) {
            // generate ELSE Block instructions
            traverse_tree(node->right, build_instruction_seq, 1);
            // Label2 (skip else)
            instruction = new_instruction(LBL, NULL, NULL, skip_else_label);
            add_instruction(instruction_seq, instruction);
        }
    }
    if (s->flag == WHILE_F) {
        symbol_t* prev_block_label = get_label();
        symbol_t* to_cond_label = get_label();
        // JMP Label2
        instruction_t* instruction = new_instruction(JMP, NULL, NULL, to_cond_label);
        add_instruction(instruction_seq, instruction);
        // Label1
        instruction = new_instruction(LBL, NULL, NULL, prev_block_label);
        add_instruction(instruction_seq, instruction);
        // generate Block instructions
        traverse_tree(node->right, build_instruction_seq, 1);
        // Label2
        instruction = new_instruction(LBL, NULL, NULL, to_cond_label);
        add_instruction(instruction_seq, instruction);
        // generate Condition instructions
        traverse_tree(node->left, build_instruction_seq, 1);
        // JMP Cond Label1
        symbol_t* condition = (symbol_t*) node->left->value;
        instruction = new_instruction(JNE, condition, NULL, prev_block_label);
        add_instruction(instruction_seq, instruction);
    }
    if (s->flag == PROC_F) {
        if (!node->middle) { // If block is null then is an extern procedure
            return;
        }

        // generate the currect procedure's out label
        curr_proc_out_lbl = get_label();

        // generate prologue
        instruction_t* prologue_inst = new_instruction(ENTER, s, NULL, NULL);
        add_instruction(instruction_seq, prologue_inst);

        // set frame size
        instruction_t* subq = new_instruction(SUB, s, create_register("rsp"), NULL);
        add_instruction(instruction_seq, subq);

        if (!strcmp(s->name, "main")) { // set the values of the global vars (if is nedeed)
            traverse_tree(global_decl_node, build_instruction_seq, 1);
        }

        // move params to registers
        int i = 0;
        symbol_t* symbol = (symbol_t*) node->value;
        list_t* params = symbol->params;
        node_t* cursor = params->head->next;
        while (cursor) {
            symbol_t* param = (symbol_t*) cursor->value;
            instruction_t* instruction = new_instruction(MOV, create_register(regs_names[i]), NULL, param);
            add_instruction(instruction_seq, instruction);
            cursor = cursor->next;
            i++;
        }

        // generate block instructions
        traverse_tree(node->middle, build_instruction_seq, 1);

        // generate epilogue
        symbol_t* proc_out = get_label();
        instruction_t* epilogue_inst = new_instruction(LEAVE, curr_proc_out_lbl, NULL, NULL);
        add_instruction(instruction_seq, epilogue_inst);
    }
    if (s->flag == CALL_F) {
        // generate arguments (possible complex expressions)
        traverse_tree(node->middle, build_instruction_seq, 1);
        int i = 0;
        list_t* args = init_list();
        enlist(node->middle, args);
        node_t* cursor = args->head->next;
        while (cursor) {
            symbol_t* param = (symbol_t*) cursor->value;
            instruction_t* instruction = new_instruction(MOV, param, NULL, create_register(regs_names[i]));
            add_instruction(instruction_seq, instruction);
            cursor = cursor->next;
            i++;
        }
        // Check if we went out of the loop because with traverse all the parameters or because we
        // need more than 6 parameters
        instruction_t* call_inst = new_instruction(CALL, s, NULL, NULL);
        add_instruction(instruction_seq, call_inst);
        instruction_t* mov_res = new_instruction(MOV, create_register("eax"), NULL, s);
        add_instruction(instruction_seq, mov_res);
    }
    if (s->flag == PROG_F) {
        // Generate the special declaration for the global vars
        list_t* glob_vars = init_list();
        enlist_vars_declaration(node->left, glob_vars);
        node_t* cursor = glob_vars->head->next;
        while (cursor) {
            symbol_t* s = (symbol_t*) cursor->value;
            s->global = 1; // Set global in true
            instruction_t* glob_decl_instr = new_instruction(GLB, s, NULL, NULL);
            add_instruction(instruction_seq, glob_decl_instr);
            cursor = cursor->next;
        }
        global_decl_node = node->left;
    }
}
