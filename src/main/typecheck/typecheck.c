#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/utils.h"
#include "typecheck.h"
#include "../npc/parser.tab.h"

char* expr_type_err_msg(int lineno, type_t operation_type, type_t operand_type) {
    char* msg = malloc(128 * sizeof(char));
    if (operation_type == INT_T) {
        sprintf(msg, "Type error in line %d - Arithmetic expressions do not accept %s type operands",
                lineno, show_type(operand_type));
    } else if (operation_type == BOOL_T) {
        sprintf(msg, "Type error in line %d - Boolean expressions do not accept %s type operands",
                lineno, show_type(operand_type));
    }
    return msg;
}

char* eq_cmp_type_err_msg(int lineno) {
    char* msg = malloc(128 * sizeof(char));
    sprintf(msg, "Type error in line %d - == comparison must be used between operands of the same type", lineno);
    return msg;
}

char* gt_lt_cmp_type_err_msg(int lineno, char* op) {
    char* msg = malloc(128 * sizeof(char));
    sprintf(msg, "Type error in line %d - %s comparison must be used between integer operands", lineno, op);
    return msg;
}

void validate_arithmetic_expression(int lineno, type_t left, type_t right) {
    if (left != INT_T) {
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

void validate_eq_comparison(int lineno, type_t left, type_t right) {
    if (left != right) {
        yyerror(eq_cmp_type_err_msg(lineno));
    }
}

void validate_gt_lt_comparison(int lineno, char* op, type_t left, type_t right) {
    if (left != INT_T || right != INT_T) {
        yyerror(gt_lt_cmp_type_err_msg(lineno, op));
    }
}

type_t validate_binary_expr(symbol_t* s, type_t left, type_t right) {
    if (!strcmp(s->name, "+") || !strcmp(s->name, "-") || !strcmp(s->name, "*")) {
        validate_arithmetic_expression(s->lineno, left, right);
        return INT_T;
    } else if (!strcmp(s->name, "||") || !strcmp(s->name, "&&")) {
        validate_boolean_expression(s->lineno, left, right);
        return BOOL_T;
    } else if (!strcmp(s->name, "==")) {
        validate_eq_comparison(s->lineno, left, right);
        return BOOL_T;
    } else if (!strcmp(s->name, "<") || !strcmp(s->name, ">")) {
        validate_gt_lt_comparison(s->lineno, s->name, left, right);
        return BOOL_T;
    } else {
        char* err_msg = malloc(128 * sizeof(char));
        sprintf(err_msg, "Undefined operation '%s'", s->name);
        yyerror(err_msg);
    }
}

type_t validate_unary_expr(symbol_t* s, type_t operand) {
    if (!strcmp(s->name, "-")) {
        if (operand != INT_T) {
            yyerror(expr_type_err_msg(s->lineno, INT_T, operand));
        }
        return INT_T;
    } else if (!strcmp(s->name, "!")) {
        if (operand != BOOL_T) {
            yyerror(expr_type_err_msg(s->lineno, BOOL_T, operand));
        }
        return BOOL_T;
    } else {
        char* err_msg = malloc(128 * sizeof(char));
        sprintf(err_msg, "Undefined operation '%s'", s->name);
        yyerror(err_msg);
    }
}

void check_types(symbol_t* s, symbol_t* left, symbol_t* right) {
    if (s->flag == BIN_OP_F) {
        s->type = validate_binary_expr(s, left->type, right->type);
    }
    if (s->flag == UN_OP_F) {
        s->type = validate_unary_expr(s, left->type); // ASSUME that the left operand es the non-null symbol
    }
    if (s->flag == ASSIGN_F || s->flag == DECL_F) {
        if (left->type != right->type) {
            yyerror(err_msg(s->lineno, left->type, right->type));
        }
    }
}
