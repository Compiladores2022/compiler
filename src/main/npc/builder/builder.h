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

#define MEM_OFFSET 4

void out_msg(int status);

symtable_t* symbol_table(symtable_t* st);

symbol_t* find_symbol(symtable_t* st, char* symbol_name);

symbol_t* build_id(symtable_t* st, char* symbol_name, type_t symbol_type, flag_t flag);

tree_node_t* build_unary_expr(char* symbol_name, tree_node_t* middle);

tree_node_t* build_binary_expr(char* symbol_name, tree_node_t* left, tree_node_t* right);

tree_node_t* build_const(type_t symbol_type, int symbol_value);

tree_node_t* build_assignment(symtable_t* st, char* symbol_name, tree_node_t* right);

tree_node_t* build_declaration(symtable_t* st, type_t symbol_type, char* symbol_name, tree_node_t* right);

tree_node_t* build_return(tree_node_t* child);

tree_node_t* link(tree_node_t* left, tree_node_t* right);

tree_node_t* build_block(tree_node_t* declarations, tree_node_t* statements);

tree_node_t* build_if(tree_node_t* expression, tree_node_t* then_block, tree_node_t* else_block);

tree_node_t* build_while(tree_node_t* expression, tree_node_t* while_block);

tree_node_t* build_param(symtable_t* st, type_t param_type, char* param_name);

tree_node_t* build_procedure_symbol(symtable_t* st, type_t proc_type, char* proc_name);

tree_node_t* build_procedure_params(tree_node_t* proc, tree_node_t* params);

tree_node_t* build_procedure(tree_node_t* proc, tree_node_t* proc_block);

tree_node_t* build_call(symtable_t* st, char* proc_name, tree_node_t* arguments);

tree_node_t* build_prog(tree_node_t* declarations, tree_node_t* procedures);


#endif
