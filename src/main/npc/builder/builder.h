#ifndef BUILDER_H
#define BUILDER_H

#include "../../symbol-table/symbol-table.h"
#include "../../symbol-list/symbol-list.h"
#include "../../syntax-tree/syntax-tree.h"
#include "../../stack/stack.h"
#include "../../list/list.h"
#include "../../symbol/symbol.h"
#include "../../tree/tree-node.h"
#include "../parser.tab.h"

void out_msg(int status);

symtable_t* symbol_table(symtable_t* st);

symbol_t* find_symbol(symtable_t* st, char* symbol_name);

symbol_t* build_id(symtable_t* st, char* symbol_name, type_t symbol_type);

tree_node_t* build_unary_expr(char* symbol_name, tree_node_t* middle);

tree_node_t* build_binary_expr(char* symbol_name, tree_node_t* left, tree_node_t* right);

tree_node_t* build_const(type_t symbol_type, int symbol_value);

tree_node_t* build_assignment(symtable_t* st, char* symbol_name, tree_node_t* right);

tree_node_t* build_declaration(symtable_t* st, type_t symbol_type, char* symbol_name, tree_node_t* right);

tree_node_t* build_return(tree_node_t* child);

tree_node_t* link(tree_node_t* left, tree_node_t* right);

#endif
