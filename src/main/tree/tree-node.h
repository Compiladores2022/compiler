#ifndef TREENODE_H
#define TREENODE_H

#include "../symbol/symbol.h"

typedef struct tree_node_s {
    void* value;
    struct tree_node_s* left;
    struct tree_node_s* middle;
    struct tree_node_s* right;
} tree_node_t;

tree_node_t* init_leaf(void* value);

tree_node_t* init_unary_tree(void* value, tree_node_t* middle);

tree_node_t* init_binary_tree(void* value, tree_node_t* left, tree_node_t* right);

void traverse_tree(tree_node_t* root, void (*f)(symbol_t*, tree_node_t*));

#endif
