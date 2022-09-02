#include <stdio.h>
#include <stdlib.h>
#include "tree-node.h"

tree_node_t* init_leaf(void* value) {
    return init_tree(value, NULL, NULL);
}

tree_node_t* init_tree(void* value, tree_node_t* left_child, tree_node_t* right_child) {
    tree_node_t* node = (tree_node_t*) malloc(sizeof(tree_node_t));
    node->value = value;
    node->left  = left_child;
    node->right = right_child;
    return node;
}
