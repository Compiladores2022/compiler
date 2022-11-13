#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "../npc/parser.tab.h"

extern list_t* procedures;

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

char* asm_filename(char* filename) {
    int len = strlen(filename);
    char* asm_filename = (char*) malloc(len * sizeof(char));
    char* res = (char*) malloc(len * sizeof(char));
    memcpy(asm_filename, filename, len-3);
    asm_filename[len - 3] = 0;
    sprintf(res, "%s.s", asm_filename);
    return res;
}

int atob(char* bool_s) {
    if (!strcmp("false", bool_s))
        return 0;

    if (!strcmp("true", bool_s))
        return 1;

    yyerror("Invalid boolean value");
}

type_t atot(char* type) {
    if (!strcmp("integer", type))
        return INT_T;

    if (!strcmp("bool", type))
        return BOOL_T;

    if (!strcmp("void", type))
        return VOID_T;

    yyerror("Invalid type name");
}

char* show_type(type_t type) {
    switch (type) {
        case INT_T:
            return "integer";
            break;
        case BOOL_T:
            return "bool";
            break;
        case VOID_T:
            return "void";
            break;
        default:
            return "wrong type";
    }
}

char* err_msg(int lineno, int expected_type, int given_type) {
    char* msg = malloc(25 * sizeof(char));
    sprintf(msg, "Expected %s but %s was found", show_type(expected_type), show_type(given_type));
    return format_err(msg, lineno);
}

char* format_err(char* err, int lineno) {
    char* output = malloc(100 * sizeof(char));
    sprintf(output, "ERROR: in line %d \n\t* %s", lineno, err);
    return output;
}

void show_tree(tree_node_t* root) {

    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->name) {
        printf("Node: %s, type: %s\n", s->name, show_type(s->type));
    } else {
        printf("Node: %d, type: %s\n", s->value, show_type(s->type));
    }
    show_tree(root->left);
    show_tree(root->right);
}

void show_params(list_t* params) {
    node_t* cursor = params->head->next;
    while(cursor) {
        symbol_t* s = (symbol_t*)cursor->value;
        printf("PARAM: %s\n", s->name);
        cursor = cursor->next;
    }
}

void enlist(tree_node_t* root, list_t* list) {
    if (root == NULL) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag != LINK_F) {
        add(list, s);
    }
    if (s->flag == UN_OP_F || s->flag == BIN_OP_F) { // if the symbol is a OP then we don't traverse the children
        return;
    }
    enlist(root->left, list);
    enlist(root->right, list);
}

void validate_main_profile(type_t type, tree_node_t* params) {
    if (type != INT_T && type != VOID_T) {
        char* err_msg = malloc(50 * sizeof(char));
        sprintf(err_msg, "Function main() can only return integer or void types");
        yyerror(format_err(err_msg, lineno()));
    }

    list_t* params_list = init_list();
    enlist(params, params_list);
    if (!is_empty(params_list)) {
        char* err_msg = malloc(50 * sizeof(char));
        sprintf(err_msg, "Cannot pass arguments to main function");
        yyerror(format_err(err_msg, lineno()));
    }
}

int is_symbol_in_list(list_t* list, symbol_t* s) {
    node_t* cursor = list->head->next;
    while (cursor) {
        symbol_t* other = (symbol_t*) cursor->value;
        if (!strcmp(s->name, other->name)) {
            return 1;
        }
        cursor = cursor->next;
    }
    return 0;
}

void register_procedure(symbol_t* s) {
    if (s->flag != PROC_F) {
        printf("register_procedure must be take a procedure symbol as parameter\n");
        exit(1);
    }
    if (search_symbol_l(procedures, s->name) != NULL) {
        return;
    }
    add_symbol(procedures, s);
}

void check_main(list_t* procedures) {
    char* err_msg = malloc(50 * sizeof(char));
    if (search_symbol_l(procedures, "main") == NULL) {
        sprintf(err_msg, "Could not find a function main");
        yyerror(format_err(err_msg, 0));
    }
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
