#include "instr-seq-gen-utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void enlist_vars_declaration(tree_node_t* root, list_t* list) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*) root->value;
    if (s->flag == DECL_F) {
        add(list, (symbol_t*) root->left->value);
    }
    enlist_vars_declaration(root->left, list);
    enlist_vars_declaration(root->right, list);
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
    if (type == ENTER) {
        return "ENTER";
    }
    if (type == LEAVE) {
        return "LEAVE";
    }
    exit(1);
}

void show_instruction_seq(list_t* instructions) {
    node_t* cursor = instructions->head->next;
    while (cursor) {
        instruction_t* instruction = (instruction_t*)cursor->value;
        printf("Instruction: \n");
        printf("Type: %s\n", type_to_str(instruction->type));
        if (instruction->s1) {
            printf("left operand name:  %s\n", instruction->s1->name);
            printf("left operand value:  %d\n", instruction->s1->value);
        }
        if (instruction->s2) {
            printf("right operand name: %s\n", instruction->s2->name);
            printf("right operand value: %d\n", instruction->s2->value);
        }
        printf("result name: %s\n", instruction->s3->name);
        printf("result value: %d\n", instruction->s3->value);
        printf("************\n");
        cursor = cursor->next;
    }
}
