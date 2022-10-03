#include "syntax-tree.h"

tree_node_t* init_leaf_s(symbol_t* s) {
    return init_leaf(s);
}

tree_node_t* init_unary_tree_s(symbol_t* s, tree_node_t* middle) {
    return init_unary_tree(s, middle);
}

tree_node_t* init_binary_tree_s(symbol_t* s, tree_node_t* left, tree_node_t* right) {
    return init_binary_tree(s, left, right);
}

symbol_t* get_symbol(tree_node_t* node) {
    return (symbol_t*) node->value;
}
