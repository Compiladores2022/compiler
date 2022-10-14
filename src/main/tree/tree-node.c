#include <stdio.h>
#include <stdlib.h>

#include "tree-node.h"
#include "../symbol/symbol.h"

tree_node_t* init_leaf(void* value) {
    tree_node_t* node = (tree_node_t*) malloc(sizeof(tree_node_t));
    node->value = value;
    return node;
}

tree_node_t* init_unary_tree(void* value, tree_node_t* middle) {
    tree_node_t* node = (tree_node_t*) malloc(sizeof(tree_node_t));
    node->value = value;
    node->middle = middle;
    return node;
}

tree_node_t* init_binary_tree(void* value, tree_node_t* left, tree_node_t* right) {
    tree_node_t* node = (tree_node_t*) malloc(sizeof(tree_node_t));
    node->value = value;
    node->left  = left;
    node->right = right;
    return node;
}

tree_node_t* init_ternary_tree(void* value, tree_node_t* left, tree_node_t* middle, tree_node_t* right) {
    tree_node_t* node = (tree_node_t*) malloc(sizeof(tree_node_t));
    node->value = value;
    node->left  = left;
    node->middle = middle;
    node->right = right;
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
        traverse_tree(root->middle, f);
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
        traverse_tree(root->middle, f);
        (*f)(s, root);
    }
    if (s->flag == PROG_F || s->flag == PROC_F || s->flag == BLOCK_F || s->flag == LINK_F) {
        traverse_tree(root->left, f);
        traverse_tree(root->right, f);
    }
}
