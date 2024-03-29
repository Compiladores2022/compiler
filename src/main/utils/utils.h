#ifndef UTILS_H
#define UTILS_H

#include "../symbol-list/symbol-list.h"
#include "../list/list.h"
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

void enlist(tree_node_t* root, list_t* params);

void validate_main_profile(type_t type, tree_node_t* params);

int is_symbol_in_list(list_t* list, symbol_t* s);

void show_params(list_t* params);

void register_procedure(symbol_t* s);

void check_main(list_t* procedures);

char* format_err(char* err, int lineno);

#endif
