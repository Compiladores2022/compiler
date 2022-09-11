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

    printf("Invalid boolean value");
    exit(1);
}

type_t atot(char* type) {
    if (!strcmp("int", type))
        return INT_T;

    if (!strcmp("bool", type))
        return BOOL_T;

    printf("Invalid type name");
    exit(1);
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
        yyerror();
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
        yyerror();
        exit(1);
    }
    return s;
}

tree_node_t* build_expression(char* symbol_name, tree_node_t* right, tree_node_t* left) {
    symbol_t* s = create_symbol();
    s->flag = OP_F;
    s->name = symbol_name;
    //printf("Creating '%s' expression\n", s->name);
    return init_tree_s(s, left, right);
}

tree_node_t* build_const(type_t symbol_type, int symbol_value) {
    symbol_t* s = create_symbol();
    s->flag = BASIC_F;
    s->type = symbol_type;
    s->value = symbol_value;
    //printf("Creating '%d' value of type %d\n", s->value, s->type);
    return init_leaf_s(s);
}

tree_node_t* build_assignment(symtable_t* st, char* symbol_name, tree_node_t* right) {
    symbol_t* symbol = find_symbol(st, symbol_name);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = ASSIGN_F;
    s->name = "=";
    //printf("Creating assignment of type %d\n", s->type);
    return init_tree_s(s, left, right);
}

tree_node_t* build_declaration(symtable_t* st, char* symbol_name, type_t symbol_type, tree_node_t* right) {
    symbol_t* symbol = build_symbol(st, symbol_name, symbol_type);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = DECL_F;
    s->name = "=";
    //printf("Creating declaration of '%s'\n", ((symbol_t*)left->value)->name);
    return init_tree_s(s, left, right);
}

tree_node_t* build_return(tree_node_t* child) {
    symbol_t* s = create_symbol();
    s->flag = RETURN_F;
    s->name = "return";
    //printf("Creating return statement of type %d\n", ((symbol_t*)(child->value))->type);
    return init_tree_s(s, child, NULL);
}

tree_node_t* link_statements(tree_node_t* left, tree_node_t* right) {
    symbol_t* s = create_symbol();
    s->flag = PROG_F;
    s->name = ";";
    //printf("Linking...\n");
    return init_tree_s(s, left, right);
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

int valid_type(symbol_t* s, type_t left, type_t right) {

    if (right != left) {
        return 0;
    }

    type_t type;

    if (!strcmp(s->name, "+") || !strcmp(s->name, "-") || !strcmp(s->name, "*")) {
        type = INT_T;
    } else if (!strcmp(s->name, "|") || !strcmp(s->name, "&")) {
        type = BOOL_T;
    } else {
        return 0;
    }

    if (type == left && type == right)
        return 1;

    return 0;
}

void check_types(tree_node_t* root) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag == ID_F || s->flag == BASIC_F) {
        return;
    }
    if (s->flag == ASSIGN_F) {
        check_types(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        if (left->type != right->type) {
            yyerror();
        }
    }
    if (s->flag == OP_F) {
        show_tree(root);
        check_types(root->left);
        check_types(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        if (valid_type(s, left->type, right->type) == 0) {
            yyerror();
        }
        s->type = left->type; //is equals to right too
    }
    if (s->flag == DECL_F) {
        if (!root->right) {
            return;
        }
        check_types(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        if (left->type != right->type) {
            yyerror();
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
