#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "../tree/tree-node.h"
#include "../list/list.h"

void build_instruction_seq(tree_node_t* root, list_t* instruction_seq);

#endif
