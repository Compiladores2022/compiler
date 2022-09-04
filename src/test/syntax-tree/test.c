#include <stdio.h>
#include "syntax-tree.h"
#include "../symbol/symbol.h"

int main() {
    symbol_t* s0 = create_symbol();
    set_name(s0, "test0");
    symbol_t* s1 = create_symbol();
    set_name(s1, "test1");
    symbol_t* s2 = create_symbol();
    set_name(s2, "test2");
    symbol_t* s3 = create_symbol();
    set_name(s3, "test3");
    symbol_t* s4 = create_symbol();
    set_name(s4, "test4");
    symbol_t* s5 = create_symbol();
    set_name(s5, "test5");
    symbol_t* s6 = create_symbol();
    set_name(s6, "test6");

    tree_node_t* leaf0 = init_leaf_s(s0);
    tree_node_t* leaf1 = init_leaf_s(s1);
    tree_node_t* leaf2 = init_leaf_s(s2);
    tree_node_t* leaf3 = init_leaf_s(s3);

    tree_node_t* sub_tree0 = init_tree_s(s4, leaf0, leaf1);
    tree_node_t* sub_tree1 = init_tree_s(s5, leaf2, leaf3);

    tree_node_t* root = init_tree_s(s6, sub_tree0, sub_tree1);

    printf("%s\n", get_symbol(root)->name);
    printf("%s\n", get_symbol(root->left)->name);
    printf("%s\n", get_symbol(root->right)->name);
    printf("%s\n", get_symbol(root->left->left)->name);
    printf("%s\n", get_symbol(root->left->right)->name);
    printf("%s\n", get_symbol(root->right->left)->name);
    printf("%s\n", get_symbol(root->right->right)->name);

    return 0;
}
