#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tree/tree-node.h"
#include "../instruction/instruction.h"
#include "../instruction/instruction-sequence.h"

inst_type_t op_to_inst_type(char* op) {
    if (!strcmp(op, "+")) {
        return ADD;
    }
    if (!strcmp(op, "-")) {
        return SUB;
    }
    if (!strcmp(op, "*")) {
        return MUL;
    }
    if (!strcmp(op, "&")) {
        return AND;
    }
    if (!strcmp(op, "|")) {
        return OR;
    }
    printf("Error - Unknown operation: %s\n", op);
    exit(1);
}

void build_instruction_seq(tree_node_t* root, list_t* instruction_seq) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag == ID_F || s->flag == BASIC_F) {
        return;
    }
    if (s->flag == OP_F) {
        build_instruction_seq(root->left, instruction_seq);
        build_instruction_seq(root->right, instruction_seq);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        inst_type_t type = op_to_inst_type(s->name);
        instruction_t* instruction = new_instruction(type, left, right, s);
        add_instruction(instruction_seq, instruction);
    }
    if (s->flag == ASSIGN_F) {
        build_instruction_seq(root->right, instruction_seq);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        instruction_t* instruction = new_instruction(MOV, right, NULL, left);
        add_instruction(instruction_seq, instruction);
    }
    if (s->flag == DECL_F) {
        if (!root->right) {
            return;
        }
        build_instruction_seq(root->right, instruction_seq);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        instruction_t* instruction = new_instruction(MOV, right, NULL, left);
        add_instruction(instruction_seq, instruction);
    }
    if (s->flag == RETURN_F) {
        build_instruction_seq(root->left, instruction_seq);
    }
    if (s->flag == PROG_F) {
        build_instruction_seq(root->left, instruction_seq);
        build_instruction_seq(root->right, instruction_seq);
    }
}

char* type_to_str(inst_type_t type) {
    if (type == ADD) {
        return "ADD";
    }
    if (type == SUB) {
        return "SUB";
    }
    if (type == MUL) {
        return "MUL";
    }
    if (type == AND) {
        return "AND";
    }
    if (type == OR) {
        return "OR";
    }
    if (type == MOV) {
        return "MOV";
    }
    exit(1);
}

void show_list(list_t* instructions) {
    node_t* cursor = instructions->head->next;
    while (cursor) {
        instruction_t* instruction = (instruction_t*)cursor->value;
        printf("Instruction: \n");
        printf("Type: %s\n", type_to_str(instruction->type));
        printf("left operand name:  %s\n", instruction->s1->name);
        printf("left operand value:  %d\n", instruction->s1->value);
        if (instruction->type != MOV) {
            printf("right operand name: %s\n", instruction->s2->name);
            printf("right operand value: %d\n", instruction->s2->value);
        }
        printf("result: %s\n", instruction->s3->name);
        printf("************\n");
        cursor = cursor->next;
    }
}
