#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "../symbol-table/symbol-table.h"
#include "../symbol-list/symbol-list.h"
#include "../syntax-tree/syntax-tree.h"
#include "../stack/stack.h"
#include "../list/list.h"
#include "../symbol/symbol.h"
#include "../tree/tree-node.h"
#include "../npc/parser.tab.h"

void yyerror(const char* msg);

void validate_arithmetic_expression(int lineno, type_t left, type_t right);

void validate_boolean_expression(int lineno, type_t left, type_t right);

void validate_expression_types(symbol_t* s, type_t left, type_t right);

void check_types(tree_node_t* root);

void show_list(list_t* instructions);

#endif
