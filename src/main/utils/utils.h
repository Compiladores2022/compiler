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

char* asm_filename(char* filename);

int atob(char* bool);

type_t atot(char* type);

char* show_type(type_t type);

char* err_msg(int lineno, int expected_type, int given_type);

void show_tree(tree_node_t* root);

list_t* enlist(tree_node_t* root, list_t* params);

int is_symbol_in_list(list_t* list, symbol_t* s);

void show_params(list_t* params);

#endif
