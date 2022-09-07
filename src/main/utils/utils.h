#ifndef UTILS_H
#define UTILS_H

#include "../symbol-table/symbol-table.h"
#include "../symbol-list/symbol-list.h"
#include "../syntax-tree/syntax-tree.h"
#include "../stack/stack.h"
#include "../list/list.h"
#include "../symbol/symbol.h"
#include "../tree/tree-node.h"



const char* extension(const char path[]);

int atob(char* bool);

symtable_t* symbol_table(symtable_t* st);

void out_msg(int status);

symbol_t* add_symbol_p(void (*error)(void), symtable_t* st, char* symbol_name, type_t symbol_type);

symbol_t* search_symbol_p(void (*error)(void), symtable_t* st, char* symbol_name);

tree_node_t* create_expression(char* symbol_name, tree_node_t* right, tree_node_t* left);

tree_node_t* create_const(type_t symbol_type, int symbol_value);

tree_node_t* create_assignment(symbol_t* symbol, tree_node_t* right);

tree_node_t* add_declaration(tree_node_t* left, tree_node_t* right);

tree_node_t* create_return(tree_node_t* child);

#endif
