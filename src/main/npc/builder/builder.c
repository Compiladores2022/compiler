#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../utils/utils.h"
#include "builder.h"
#include "../parser.tab.h"

symtable_t* symbol_table(symtable_t* st) {
    st = init_symtable();
    push_level(st);
    return st;
}

void out_msg(int status) {
    if (status) {
        printf("There were compile-time errors!\n");
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
    return init_leaf_s(s);
}

tree_node_t* build_expression(char* symbol_name, tree_node_t* left, tree_node_t* right) {
    symbol_t* s = create_symbol();
    s->flag = OP_F;
    s->name = symbol_name;
    s->lineno = lineno();
    return init_tree_s(s, left, right);
}

tree_node_t* build_assignment(symtable_t* st, char* symbol_name, tree_node_t* right) {
    symbol_t* symbol = find_symbol(st, symbol_name);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = ASSIGN_F;
    s->name = "=";
    s->lineno = lineno();
    return init_tree_s(s, left, right);
}

tree_node_t* build_declaration(symtable_t* st, char* symbol_name, type_t symbol_type, tree_node_t* right) {
    symbol_t* symbol = build_symbol(st, symbol_name, symbol_type);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = DECL_F;
    s->name = "=";
    s->lineno = lineno();
    return init_tree_s(s, left, right);
}

tree_node_t* build_return(tree_node_t* child) {
    symbol_t* s = create_symbol();
    s->flag = RETURN_F;
    s->name = "return";
    s->lineno = lineno();
    return init_tree_s(s, child, NULL);
}

tree_node_t* link_statements(tree_node_t* left, tree_node_t* right) {
    symbol_t* s = create_symbol();
    s->flag = PROG_F;
    s->name = ";";
    return init_tree_s(s, left, right);
}