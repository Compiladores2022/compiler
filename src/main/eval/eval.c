#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "eval.h"
#include "../utils/utils.h"
#include "../npc/parser.tab.h"

void evaluate_expression(symbol_t* s, symbol_t* left, symbol_t* right) { // when we add new types for example float, we must change this
    if (s->type == INT_T) {
        if (!strcmp(s->name, "+")) {
            s->value = left->value + right->value;
        } else if (!strcmp(s->name, "*")) {
            s->value = left->value * right->value;
        } else if (!strcmp(s->name, "-")) {
            s->value = left->value - right->value;
        }
    } else if (s->type == BOOL_T) {
        if (!strcmp(s->name, "&")) {
            s->value = left->value && right->value;
        } else if (!strcmp(s->name, "|")) {
            s->value = left->value || right->value;
        }
    }
}

void evaluate(tree_node_t* root) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag == ID_F || s->flag == BASIC_F) {
        return;
    }
    if (s->flag == OP_F) {
        show_tree(root);
        evaluate(root->left);
        evaluate(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        evaluate_expression(s, left, right);
    }
    if (s->flag == ASSIGN_F) {
        evaluate(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        left->value = right->value;
        printf("Assigning: %d\n", left->value);
    }
    if (s->flag == DECL_F) {
        if (!root->right) {
            return;
        }
        evaluate(root->right);
        symbol_t* left = (symbol_t*)(root->left->value);
        symbol_t* right = (symbol_t*)(root->right->value);
        left->value = right->value;
        printf("Assigning: %d\n", left->value);
    }
    if (s->flag == RETURN_F) {
        evaluate(root->left);
        s->value = ((symbol_t*) root->left->value)->value;
        printf("Returning: %d\n", s->value);
    }
    if (s->flag == PROG_F) {
        evaluate(root->left);
        evaluate(root->right);
    }
}
