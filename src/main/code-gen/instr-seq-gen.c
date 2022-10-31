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
    if (!strcmp(op, "/")) {
        return DIV;
    }
    if (!strcmp(op, "%")) {
        return MOD;
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

int labels_counter = 0;

symbol_t* get_label() {
    symbol_t* s = create_symbol();
    labels_counter++;
    char* name = (char*) malloc(sizeof(char) * 10);
    sprintf(name, ".L%d", labels_counter);
    s->name = name;
    s->flag = LABEL_F;
    return s;
}

symbol_t* create_register(char* reg_name) {
    symbol_t* s = create_symbol();
    s->name = reg_name;
    s->flag = REG_F;
    return s;
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
        if (node->middle) {
            symbol_t* middle = (symbol_t*) node->middle->value;
            instruction_t* instruction = new_instruction(RET, NULL, NULL, middle);
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
    if (s->flag == CALL_F) {
        const char *a[2];
        a[0] = "blah";
        a[1] = "hmm";
        //  int i = 0;
        //  for each p in s->params
        //      mov p to reg[i]
        //      i++
        //  call s->name
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
    if (type == DIV) {
        return "DIV";
    }
    if (type == MOD) {
        return "MOD";
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
    if (type == JMP) {
        return "JMP";
    }
    if (type == JE) {
        return "JE";
    }
    if (type == JNE) {
        return "JNE";
    }
    if (type == LBL) {
        return "LBL";
    }
    exit(1);
}

void show_list(list_t* instructions) {
    node_t* cursor = instructions->head->next;
    while (cursor) {
        instruction_t* instruction = (instruction_t*)cursor->value;
        /* printf("Instruction: \n"); */
        /* printf("Type: %s\n", type_to_str(instruction->type)); */
        if (instruction->s1) {
            /* printf("left operand name:  %s\n", instruction->s1->name); */
            /* printf("left operand value:  %d\n", instruction->s1->value); */
        }
        if (instruction->s2) {
            /* printf("right operand name: %s\n", instruction->s2->name); */
            /* printf("right operand value: %d\n", instruction->s2->value); */
        }
        /* printf("result name: %s\n", instruction->s3->name); */
        /* printf("result value: %d\n", instruction->s3->value); */
        /* printf("************\n"); */
        cursor = cursor->next;
    }
}
