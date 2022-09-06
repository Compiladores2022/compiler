#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

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

int atob(char* bool) {

    if (!strcmp("false", bool))
        return 0;

    if (!strcmp("true", bool))
        return 1;

    printf("Invalid boolean value");
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

symbol_t* add_symbol_p(void (*f)(void), symtable_t* st, char* symbol_name, type_t symbol_type) {
    symbol_t* s;
    if (search_symbol(st, symbol_name) == NULL) {
        s = create_symbol();
        s->name = symbol_name;
        s->type = symbol_type;
        insert_symbol(st, s);
        printf("Identifier '%s' of type %d was added\n", s->name, s->type);
    } else {
        printf("Error - Identifier '%s' is trying to be re-declared\n", symbol_name);
        (*f)();
        exit(1);
    }
    return s;
}

symbol_t* search_symbol_p(void (*error)(void), symtable_t* st, char* symbol_name) {
    symbol_t* s = search_symbol(st, symbol_name);
    if (s == NULL) {
        printf("Error - Undeclared identifier '%s'\n", symbol_name);
        (*error)();
    } else {
        printf("Identifier '%s' was found\n", s->name);
    }
    return s;
}

tree_node_t* create_expression(char* symbol_name, tree_node_t* right, tree_node_t* left) {
    symbol_t* s = create_symbol();
    s->flag = OP_F;
    s->name = symbol_name;
    printf("Creating '%s' expression\n", s->name);
    tree_node_t* node = init_tree_s(s, left, right);
    return node;
}

tree_node_t* create_const(type_t symbol_type, int symbol_value) {
    symbol_t* s = create_symbol();
    s->flag = BASIC_F;
    s->type = symbol_type;
    s->value = symbol_value;
    printf("Creating '%d' value of type %d\n", s->value, s->type);
    tree_node_t* node = init_leaf_s(s);
    return node;
}

tree_node_t* create_assignment(symbol_t* symbol, tree_node_t* right) {
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = ASSIGN_F;
    s->name = "=";
    s->type = symbol->type;
    printf("Creating assignment of type %d\n", symbol->type);
    tree_node_t* node = init_tree_s(s, left, right);
    return node;
}

tree_node_t* add_declaration(symbol_t* symbol, tree_node_t* left, tree_node_t* right) {
    symbol_t* s = create_symbol();
    s->flag = DECL_F;
    s->type = symbol->type;
    s->name = "=";
    printf("Creating declaration of '%s' of type %d\n", symbol->name, symbol->type);
    tree_node_t* node = init_tree_s(s, left, right);
    return node;
}
