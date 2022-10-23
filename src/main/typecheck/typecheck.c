#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/utils.h"
#include "typecheck.h"
#include "../npc/parser.tab.h"
#include "../npc/builder/builder.h"

extern int glob_offset;

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

char* diff_param_arg_type_err_msg(type_t param_type, type_t arg_type, char* proc_name, int lineno, int param_index) {
    char* msg = (char*) malloc(100 * sizeof(char));
    sprintf(msg, "%s for %s in the %dth parameter", err_msg(lineno, param_type, arg_type), proc_name, param_index);
    return msg;
}

char* diff_size_of_params_args(node_t* params_cursor, node_t* args_cursor, char* proc_name, int lineno) {
    char* msg = (char*) malloc(100 * sizeof(char));
    if (params_cursor) {
        sprintf(msg, "Arguments missing for %s in line %d", proc_name, lineno);
    }
    if (args_cursor) {
        sprintf(msg, "Too many arguments for %s in line %d", proc_name, lineno);
    }
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
    if (!strcmp(s->name, "+") || !strcmp(s->name, "-") || !strcmp(s->name, "*") 
            || !strcmp(s->name, "/") || !strcmp(s->name, "%")) {
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

void validate_proc(symbol_t* s, tree_node_t* node) {
    if (node == NULL) {
        return;
    }
    symbol_t* node_sym = (symbol_t*) node->value;
    if (node_sym->flag == RETURN_F) {
        type_t expected_type = s->type;
        type_t given_type = node_sym->type;
        if (expected_type != given_type) {
            yyerror(err_msg(node_sym->lineno, expected_type, given_type));
        }
    }
    validate_proc(s, node->left);
    validate_proc(s, node->middle);
    validate_proc(s, node->right);
}

void validate_params(symbol_t* s, tree_node_t* args) {
    list_t* params_list = s->params;
    list_t* args_list = init_list();
    args_list = enlist(args, args_list);
    node_t* params_cursor = params_list->head->next;
    node_t* args_cursor = args_list->head->next;
    int param_index = 1;
    while(params_cursor && args_cursor) {
        symbol_t* param = (symbol_t*) params_cursor->value;
        symbol_t* arg = (symbol_t*) args_cursor->value;
        if (param->type != arg->type) {
            yyerror(diff_param_arg_type_err_msg(param->type, arg->type, s->name, s->lineno, param_index));
        }
        params_cursor = params_cursor->next;
        args_cursor = args_cursor->next;
        param_index++;
    }
    if (params_cursor || args_cursor) {
        yyerror(diff_size_of_params_args(params_cursor, args_cursor, s->name, s->lineno));
    }
}

int frame_size(tree_node_t* node) {

    int count = 0;

    if (!node) {
        return 0;
    }

    symbol_t* s = (symbol_t*) node->value;
    if (s->flag == DECL_F || s->flag == UN_OP_F || s->flag == BIN_OP_F) {
       count++;
    }

    count += frame_size(node->left);
    count += frame_size(node->middle);
    count += frame_size(node->right);

    return count;
}

void check_types(symbol_t* s, tree_node_t* node) {
    if (s->flag == BIN_OP_F) {
        symbol_t* left = (symbol_t*) node->left->value;
        symbol_t* right = (symbol_t*) node->right->value;
        s->type = validate_binary_expr(s, left->type, right->type);
    }
    if (s->flag == UN_OP_F) {
        symbol_t* middle = (symbol_t*) node->middle->value;
        s->type = validate_unary_expr(s, middle->type); // ASSUME that the left operand es the non-null symbol
    }
    if (s->flag == ASSIGN_F || s->flag == DECL_F) {
        symbol_t* left = (symbol_t*) node->left->value;
        symbol_t* right = (symbol_t*) node->right->value;
        if (left->type != right->type) {
            yyerror(err_msg(s->lineno, left->type, right->type));
        }
    }
    if (s->flag == RETURN_F) {
        if (node->middle != NULL) {
            s->type = ((symbol_t*) node->middle->value)->type;
        } else {
            s->type = VOID_T; // If a return has no a expression the returns void;
        }
    }
    if (s->flag == IF_F) {
        symbol_t* cond = (symbol_t*) node->middle->value;
        if (cond->type != BOOL_T) {
            yyerror(err_msg(cond->lineno, BOOL_T, cond->type));
        }
    }
    if (s->flag == WHILE_F) {
        symbol_t* cond = (symbol_t*) node->left->value;
        if (cond->type != BOOL_T) {
            yyerror(err_msg(cond->lineno, BOOL_T, cond->type));
        }
    }
    if (s->flag == PROC_F) {
        printf("IN PROC ********************** \n");
        validate_proc(s, node->right); // node->right because there is the block
        int size = frame_size(node->right);
        printf("GLOB: %d \n", glob_offset);
        printf("MEM: %d \n", MEM_OFFSET);
        printf("PROC OFFSET: %d \n", s->offset);
        printf("SIZE: %d \n", size);
        glob_offset = glob_offset - size;
        s->offset = glob_offset * MEM_OFFSET;
    }
    if (s->flag == CALL_F) {
        validate_params(s, node->middle);
    }
}
