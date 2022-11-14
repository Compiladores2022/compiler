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
        if (!strcmp(s->name, "&&")) {
            s->value = left->value && right->value;
        } else if (!strcmp(s->name, "||")) {
            s->value = left->value || right->value;
        }
    }
}

void evaluate(symbol_t* s, symbol_t* left, symbol_t* right) {
    if (s->flag == BIN_OP_F) {
        evaluate_expression(s, left, right);
    }
    if (s->flag == ASSIGN_F || s->flag == DECL_F) {
        left->value = right->value;
    }
    if (s->flag == RETURN_F) {
        s->value = left->value;
        printf("%d\n", s->value);
    }
}
