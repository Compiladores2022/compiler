#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "../npc/parser.tab.h"

const char* extension(const char path[])
{
    const char *result;
    int i, n;
    n = strlen(path);
    i = n - 1;
    while ((i >= 0) && (path[i] != '.') && (path[i] != '/') & (path[i] != '\\')) {
        i--;
    }
    if ((i > 0) && (path[i] == '.') && (path[i - 1] != '/') && (path[i - 1] != '\\')) {
        result = path + i;
    } else {
        result = path + n;
    }
    return result;
}

int atob(char* bool_s) {
    if (!strcmp("false", bool_s))
        return 0;

    if (!strcmp("true", bool_s))
        return 1;

    yyerror("Invalid boolean value");
}

type_t atot(char* type) {
    if (!strcmp("int", type))
        return INT_T;

    if (!strcmp("bool", type))
        return BOOL_T;

    yyerror("Invalid type name");
}

char* show_type(type_t type) {
    switch (type) {
        case INT_T:
            return "int";
            break;
        case BOOL_T:
            return "bool";
            break;
        default:
            return "wrong type";
    }
}

char* err_msg(int lineno, int expected_type, int given_type) {
    char* msg = malloc(128 * sizeof(char));
    sprintf(msg, "Type error in line %d - Expected %s but %s was found", 
            lineno, show_type(expected_type), show_type(given_type));
    return msg;
}

void show_tree(tree_node_t* root) {
    if (!root) {
        return;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->name) {
        /* printf("Node: %s, type: %s\n", s->name, show_type(s->type)); */
    } else {
        /* printf("Node: %d, type: %s\n", s->value, show_type(s->type)); */
    }
    show_tree(root->left);
    show_tree(root->right);
}
