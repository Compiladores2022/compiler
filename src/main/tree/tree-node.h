#ifndef TREENODE_H
#define TREENODE_H

typedef struct tree_node_s {
    void* value;
    struct tree_node_s* left;
    struct tree_node_s* right;
} tree_node_t;

tree_node_t* init_leaf(void* value);

tree_node_t* init_tree(void* value, tree_node_t* left_child, tree_node_t* right_child);

#endif
