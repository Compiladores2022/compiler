#include <stdio.h>
#include <stdlib.h>

#include "tree-node.h"
#include "../symbol/symbol.h"

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

void traverse_tree(tree_node_t* root, void (*f)(symbol_t*, tree_node_t*)) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag == ID_F || s->flag == BASIC_F) {
        return;
    }
    if (s->flag == BIN_OP_F) {
        traverse_tree(root->left, f);
        traverse_tree(root->right, f);
        (*f)(s, root);
    }
    if (s->flag == UN_OP_F) {
        traverse_tree(root->left, f);
        (*f)(s, root);
    }
    if (s->flag == ASSIGN_F) {
        traverse_tree(root->right, f);
        (*f)(s, root);
    }
    if (s->flag == DECL_F) {
        if (!root->right) {
            return;
        }
        traverse_tree(root->right, f);
        (*f)(s, root);
    }
    if (s->flag == RETURN_F) {
        traverse_tree(root->left, f);
        (*f)(s, root);
    }
    if (s->flag == PROG_F) {
        traverse_tree(root->left, f);
        traverse_tree(root->right, f);
    }
}
