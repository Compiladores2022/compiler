#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../tree/tree-node.h"
#include "../instruction/instruction.h"
#include "../instruction/instruction-sequence.h"

list_t* instruction_seq;

instr_type_t bin_op_to_instr_type(char* op) {
    if (!strcmp(op, "+")) {
        return ADD;
    }
    if (!strcmp(op, "-")) {
        return SUB;
    }
    if (!strcmp(op, "*")) {
        return MUL;
    }
    if (!strcmp(op, "&&")) {
        return AND;
    }
    if (!strcmp(op, "||")) {
        return OR;
    }
    if (!strcmp(op, "==")) {
        return EQ;
    }
    if (!strcmp(op, ">")) {
        return GT;
    }
    if (!strcmp(op, "<")) {
        return LT;
    }
    printf("Error while generating 3D instruction - Unknown operation: %s\n", op);
    exit(1);
}

instr_type_t un_op_to_instr_type(char* op) {
    if (!strcmp(op, "-")) {
        return MIN;
    }
    if (!strcmp(op, "!")) {
        return NEG;
    }
    printf("Error while generating 3D instruction - Unknown operation: %s\n", op);
    exit(1);
}

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
        symbol_t* middle = (symbol_t*) node->middle->value;
        instruction_t* instruction = new_instruction(RET, NULL, NULL, middle);
        add_instruction(instruction_seq, instruction);
    }
}

char* type_to_str(instr_type_t type) {
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
    if (type == EQ) {
        return "EQ";
    }
    if (type == GT) {
        return "GT";
    }
    if (type == LT) {
        return "LT";
    }
    if (type == MIN) {
        return "MIN";
    }
    if (type == NEG) {
        return "NEG";
    }
    if (type == MOV) {
        return "MOV";
    }
    if (type == RET) {
        return "RET";
    }
    exit(1);
}

void show_list(list_t* instructions) {
    node_t* cursor = instructions->head->next;
    while (cursor) {
        instruction_t* instruction = (instruction_t*)cursor->value;
        printf("Instruction: \n");
        printf("Type: %s\n", type_to_str(instruction->type));
        if (instruction->type != RET) {
            printf("left operand name:  %s\n", instruction->s1->name);
            printf("left operand value:  %d\n", instruction->s1->value);
            if (instruction->type != MOV && instruction->type != MIN && instruction->type != NEG) {
                printf("right operand name: %s\n", instruction->s2->name);
                printf("right operand value: %d\n", instruction->s2->value);
            }
        }
        printf("result name: %s\n", instruction->s3->name);
        printf("result value: %d\n", instruction->s3->value);
        printf("************\n");
        cursor = cursor->next;
    }
}

