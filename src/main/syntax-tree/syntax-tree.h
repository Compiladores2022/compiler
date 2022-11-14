#ifndef SYNTAX_TREENODE_H
#define SYNTAX_TREENODE_H

#include "../symbol/symbol.h"
#include "../tree/tree-node.h"

tree_node_t* init_leaf_s(symbol_t* s);

tree_node_t* init_unary_tree_s(symbol_t* s, tree_node_t* middle);

tree_node_t* init_binary_tree_s(symbol_t* s, tree_node_t* left, tree_node_t* right);

tree_node_t* init_ternary_tree_s(symbol_t* s, tree_node_t* left, tree_node_t* middle, tree_node_t* right);

symbol_t* get_symbol(tree_node_t* node);

#endif
