#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "../npc/parser.tab.h"

const char* extension(const char path[])
{
    const char *result;
    int i, n;
    n = strlen(path);
    i = n - 1;
    while ((i >= 0) && (path[i] != '.') && (path[i] != '/') & (path[i] != '\\')) {
        i--;
    }
    if ((i > 0) && (path[i] == '.') && (path[i - 1] != '/') && (path[i - 1] != '\\')) {
        result = path + i;
    } else {
        result = path + n;
    }
    return result;
}

int atob(char* bool_s) {
    if (!strcmp("false", bool_s))
        return 0;

    if (!strcmp("true", bool_s))
        return 1;

    yyerror("Invalid boolean value");
}

type_t atot(char* type) {
    if (!strcmp("int", type))
        return INT_T;

    if (!strcmp("bool", type))
        return BOOL_T;

    yyerror("Invalid type name");
}

char* show_type(type_t type) {
    switch (type) {
        case INT_T:
            return "int";
            break;
        case BOOL_T:
            return "bool";
            break;
        default:
            return "wrong type";
    }
}

char* err_msg(int lineno, int expected_type, int given_type) {
    char* msg = malloc(128 * sizeof(char));
    sprintf(msg, "Type error in line %d - Expected %s but %s was found", 
            lineno, show_type(expected_type), show_type(given_type));
    return msg;
}

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

symtable_t* symbol_table(symtable_t* st) {
    st = init_symtable();
    push_level(st);
    return st;
}

void out_msg(int status) {
    switch (status) {
        case 0:
            printf("No errors were found!\n");
            break;
        default:
            printf("There were compile-time errors!\n");
            break;
    }
}


symbol_t* find_symbol(symtable_t* st, char* symbol_name) {
    symbol_t* s = search_symbol(st, symbol_name);
    if (s == NULL) {
        //printf("Error - Undeclared identifier '%s'\n", symbol_name);
        yyerror("Undeclared identifier");
    } else {
        //printf("Identifier '%s' was found\n", s->name);
    }
    return s;
}

symbol_t* build_symbol(symtable_t* st, char* symbol_name, type_t symbol_type) {
    symbol_t* s;
    if (search_symbol(st, symbol_name) == NULL) {
        s = create_symbol();
        s->name = symbol_name;
        s->type = symbol_type;
        insert_symbol(st, s);
        //printf("Identifier '%s' of type %d was added\n", s->name, s->type);
    } else {
        printf("Error - Identifier '%s' is trying to be re-declared\n", symbol_name);
        yyerror("Trying to re-declare an identifier");
    }
    return s;
}

tree_node_t* build_const(type_t symbol_type, int symbol_value) {
    symbol_t* s = create_symbol();
    s->flag = BASIC_F;
    s->type = symbol_type;
    s->value = symbol_value;
    //printf("Creating '%d' value of type %d\n", s->value, s->type);
    return init_leaf_s(s);
}

tree_node_t* build_expression(char* symbol_name, tree_node_t* right, tree_node_t* left) {
    symbol_t* s = create_symbol();
    s->flag = OP_F;
    s->name = symbol_name;
    s->lineno = lineno();
    //printf("Creating '%s' expression\n", s->name);
    return init_tree_s(s, left, right);
}

tree_node_t* build_assignment(symtable_t* st, char* symbol_name, tree_node_t* right) {
    symbol_t* symbol = find_symbol(st, symbol_name);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = ASSIGN_F;
    s->name = "=";
    s->lineno = lineno();
    printf("Assignment at line no.: %d\n", lineno());
    return init_tree_s(s, left, right);
}

tree_node_t* build_declaration(symtable_t* st, char* symbol_name, type_t symbol_type, tree_node_t* right) {
    symbol_t* symbol = build_symbol(st, symbol_name, symbol_type);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = DECL_F;
    s->name = "=";
    s->lineno = lineno();
    //printf("Creating declaration of '%s'\n", ((symbol_t*)left->value)->name);
    printf("Declaration at line no.: %d\n", lineno());
    return init_tree_s(s, left, right);
}

tree_node_t* build_return(tree_node_t* child) {
    symbol_t* s = create_symbol();
    s->flag = RETURN_F;
    s->name = "return";
    s->lineno = lineno();
    //printf("Creating return statement of type %d\n", ((symbol_t*)(child->value))->type);
    printf("Return statement at line no.: %d\n", lineno());
    return init_tree_s(s, child, NULL);
}

tree_node_t* link_statements(tree_node_t* left, tree_node_t* right) {
    symbol_t* s = create_symbol();
    s->flag = PROG_F;
    s->name = ";";
    //printf("Linking...\n");
    return init_tree_s(s, left, right);
}

void show_tree(tree_node_t* root) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->name) {
        /* printf("Node: %s, type: %s\n", s->name, show_type(s->type)); */
    } else {
        /* printf("Node: %d, type: %s\n", s->value, show_type(s->type)); */
    }
    show_tree(root->left);
    show_tree(root->right);
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
        show_tree(root);
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

void evaluate_expression(symbol_t* s, symbol_t* left, symbol_t* right) { // when we add new types for example float, we must change this
    if (s->type == INT_T) {
        if (!strcmp(s->name, "+")) {
            s->value = left->value + right->value;
        } else if (!strcmp(s->name, "*")) {
            s->value = left->value * right->value;
        } else if (!strcmp(s->name, "-")) {
            s->value = left->value - right->value;
        }
    } else if (s->type == BOOL_T) {
        if (!strcmp(s->name, "&")) {
            s->value = left->value && right->value;
        } else if (!strcmp(s->name, "|")) {
            s->value = left->value || right->value;
        }
    }
}

void evaluate(tree_node_t* root) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag == ID_F || s->flag == BASIC_F) {
        return;
    }
    if (s->flag == OP_F) {
        show_tree(root);
        evaluate(root->left);
        evaluate(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        evaluate_expression(s, left, right);
    }
    if (s->flag == ASSIGN_F) {
        evaluate(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        left->value = right->value;
        printf("Assigning: %d\n", left->value);
    }
    if (s->flag == DECL_F) {
        if (!root->right) {
            return;
        }
        evaluate(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        left->value = right->value;
        printf("Assigning: %d\n", left->value);
    }
    if (s->flag == RETURN_F) {
        evaluate(root->left);
        s->value = ((symbol_t*) root->left->value)->value;
        printf("Returning: %d\n", s->value);
    }
    if (s->flag == PROG_F) {
        evaluate(root->left);
        evaluate(root->right);
    }
}
