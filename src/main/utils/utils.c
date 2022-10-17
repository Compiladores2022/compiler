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

char* asm_filename(char* filename) {
    int len = strlen(filename);
    char* asm_filename = (char*) malloc(len * sizeof(char));
    char* res = (char*) malloc(len * sizeof(char));
    memcpy(asm_filename, filename, len-3);
    asm_filename[len - 3] = 0;
    sprintf(res, "%s.s", asm_filename);
    return res;
}

int atob(char* bool_s) {
    if (!strcmp("false", bool_s))
        return 0;

    if (!strcmp("true", bool_s))
        return 1;

    yyerror("Invalid boolean value");
}

type_t atot(char* type) {
    if (!strcmp("integer", type))
        return INT_T;

    if (!strcmp("bool", type))
        return BOOL_T;

    if (!strcmp("void", type))
        return VOID_T;

    yyerror("Invalid type name");
}

char* show_type(type_t type) {
    switch (type) {
        case INT_T:
            return "integer";
            break;
        case BOOL_T:
            return "bool";
            break;
        case VOID_T:
            return "void";
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
        printf("Node: %s, type: %s\n", s->name, show_type(s->type));
    } else {
        printf("Node: %d, type: %s\n", s->value, show_type(s->type));
    }
    show_tree(root->left);
    show_tree(root->right);
}

void show_params(list_t* params) {
    node_t* cursor = params->head->next;
    while(cursor) {
        symbol_t* s = (symbol_t*)cursor->value;
        printf("PARAM: %s\n", s->name);
        cursor = cursor->next;
    }
}

list_t* enlist(tree_node_t* root, list_t* params) {
    if (root == NULL) {
        return params;
    }
    symbol_t* s = (symbol_t*)(root->value);
    if (s->flag != LINK_F) {
        add(params, s);
    }
    enlist(root->left, params);
    enlist(root->right, params);
}
