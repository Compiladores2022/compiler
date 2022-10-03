#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../utils/utils.h"
#include "builder.h"
#include "../parser.tab.h"

#define MEM_OFFSET 4

int glob_offset = -1;

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
        yyerror("Undeclared identifier");
    }
    return s;
}

symbol_t* build_id(symtable_t* st, char* symbol_name, type_t symbol_type) {
    symbol_t* s = NULL;
    if (search_symbol(st, symbol_name) == NULL) {
        s = create_symbol();
        s->name = symbol_name;
        s->flag = ID_F;
        s->type = symbol_type;
        s->value = 0; // Default value
        s->offset = (glob_offset--) * MEM_OFFSET;
        printf("id: %s, OFFSET: %d, glob: %d \n", s->name, s->offset, glob_offset);
        insert_symbol(st, s);
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
    if (left != NULL && right != NULL) {
        s->flag = BIN_OP_F;
    } else if (left != NULL ^ right != NULL) {
        s->flag = UN_OP_F;
    } else {
        yyerror("Invalid arity of expression");
    }
    s->name = symbol_name;
    s->lineno = lineno();
    s->offset = (glob_offset--) * MEM_OFFSET;
    printf("id: %s, OFFSET: %d, glob: %d \n", s->name, s->offset, glob_offset);
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
    symbol_t* symbol = build_id(st, symbol_name, symbol_type);
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
