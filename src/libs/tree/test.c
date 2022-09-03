#include <stdio.h>
#include "tree-node.h"

int main() {
    int var0 = 0;
    int var1 = 1;
    int var2 = 2;
    int var3 = 3;
    int var4 = 4;
    int var5 = 5;
    int var6 = 6;

    tree_node_t* leaf0 = init_leaf(&var0);
    tree_node_t* leaf1 = init_leaf(&var1);
    tree_node_t* leaf2 = init_leaf(&var2);
    tree_node_t* leaf3 = init_leaf(&var3);

    tree_node_t* sub_tree0 = init_tree(&var4, leaf0, leaf1);
    tree_node_t* sub_tree1 = init_tree(&var5, leaf2, leaf3);

    tree_node_t* root = init_tree(&var6, sub_tree0, sub_tree1);

    printf("%d\n", *((int*) root->value));
    printf("%d\n", *((int*) root->left->value));
    printf("%d\n", *((int*) root->right->value));
    printf("%d\n", *((int*) root->left->left->value));
    printf("%d\n", *((int*) root->left->right->value));
    printf("%d\n", *((int*) root->right->left->value));
    printf("%d\n", *((int*) root->right->right->value));


    return 0;
}
