#ifndef UTILS_H
#define UTILS_H

#include "../symbol-table/symbol-table.h"
#include "../symbol-list/symbol-list.h"
#include "../syntax-tree/syntax-tree.h"
#include "../stack/stack.h"
#include "../list/list.h"
#include "../symbol/symbol.h"
#include "../tree/tree-node.h"
#include "../npc/parser.tab.h"

void yyerror(const char* msg);

int lineno();

const char* extension(const char path[]);

int atob(char* bool);

type_t atot(char* type);

symtable_t* symbol_table(symtable_t* st);

void out_msg(int status);

symbol_t* find_symbol(symtable_t* st, char* symbol_name);

symbol_t* build_symbol(symtable_t* st, char* symbol_name, type_t symbol_type);

tree_node_t* build_expression(char* symbol_name, tree_node_t* right, tree_node_t* left);

tree_node_t* build_const(type_t symbol_type, int symbol_value);

tree_node_t* build_assignment(symtable_t* st, char* symbol_name, tree_node_t* right);

tree_node_t* build_declaration(symtable_t* st, char* symbol_name, type_t symbol_type, tree_node_t* right);

tree_node_t* build_return(tree_node_t* child);

tree_node_t* link_statements(tree_node_t* left, tree_node_t* right);

void show_tree(tree_node_t* root);

int valid_type(symbol_t* s, type_t left, type_t right);

void check_types(tree_node_t* root);

void evaluate(tree_node_t* root);

#endif
