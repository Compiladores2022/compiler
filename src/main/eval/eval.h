#ifndef EVAL_H
#define EVAL_H

#include "../symbol-table/symbol-table.h"
#include "../symbol-list/symbol-list.h"
#include "../syntax-tree/syntax-tree.h"
#include "../stack/stack.h"
#include "../list/list.h"
#include "../symbol/symbol.h"
#include "../tree/tree-node.h"
#include "../npc/parser.tab.h"

void evaluate(tree_node_t* root);

#endif
