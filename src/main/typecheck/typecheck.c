#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../utils/utils.h"
#include "typecheck.h"
#include "../npc/parser.tab.h"
#include "../instruction/instruction.h"
#include "../instruction/instruction-sequence.h"

char* expr_type_err_msg(int lineno, type_t operation_type, type_t operand_type) {
    char* msg = malloc(128 * sizeof(char));
    if (operation_type == INT_T) { // Eventualy we must add FLOAT_T here
        sprintf(msg, "Type error in line %d - Arithmetic expressions do not accept %s type operands",
                lineno, show_type(operand_type));
    } else if (operation_type == BOOL_T) {
        sprintf(msg, "Type error in line %d - Boolean expressions do not accept %s type operands",
                lineno, show_type(operand_type));
    }
    return msg;
}

void validate_arithmetic_expression(int lineno, type_t left, type_t right) {
    if (left != INT_T) { // eventually we will add FLOAT_T
        yyerror(expr_type_err_msg(lineno, INT_T, left));
    }
    if (right != INT_T) {
        yyerror(expr_type_err_msg(lineno, INT_T, right));
    }
}

void validate_boolean_expression(int lineno, type_t left, type_t right) {
    if (left != BOOL_T) {
        yyerror(expr_type_err_msg(lineno, BOOL_T, left));
    }
    if (right != BOOL_T) {
        yyerror(expr_type_err_msg(lineno, BOOL_T, right));
    }
}

void validate_expression_types(symbol_t* s, type_t left, type_t right) {
    type_t type;
    if (!strcmp(s->name, "+") || !strcmp(s->name, "-") || !strcmp(s->name, "*")) {
        validate_arithmetic_expression(s->lineno, left, right);
    } else if (!strcmp(s->name, "|") || !strcmp(s->name, "&")) {
        validate_boolean_expression(s->lineno, left, right);
    } else {
        char* err_msg = malloc(128 * sizeof(char));
        sprintf(err_msg, "Undefined operation '%s'", s->name);
        yyerror(err_msg);
    }
}

void check_types(tree_node_t* root) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag == ID_F || s->flag == BASIC_F) {
        return;
    }
    if (s->flag == OP_F) {
        check_types(root->left);
        check_types(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        validate_expression_types(s, left->type, right->type);
        s->type = left->type; //is equals to right too
    }
    if (s->flag == ASSIGN_F) {
        check_types(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        if (left->type != right->type) {
            yyerror(err_msg(s->lineno, left->type, right->type));
        }
    }
    if (s->flag == DECL_F) {
        if (!root->right) {
            return;
        }
        check_types(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        if (left->type != right->type) {
            yyerror(err_msg(s->lineno, left->type, right->type));
        }
    }
    if (s->flag == RETURN_F) {
        check_types(root->left);
    }
    if (s->flag == PROG_F) {
        check_types(root->left);
        check_types(root->right);
    }
}

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
