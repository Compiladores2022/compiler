#ifndef UTILS_H
#define UTILS_H

// MACROS

#define SYMBOL_TABLE() {                        \
        st = init_symtable();                   \
        push_level(st);                         \
    }

#define NO_ERRORS() {                           \
        printf("No errors were found! \n");     \
    }

#define ADD_SYMBOL(symbol_name) ({                                              \
    symbol_t* s;                                                                \
    if (search_symbol(st, symbol_name) == NULL) {                               \
        s = create_symbol();                                                    \
        s->name = symbol_name;                                                  \
        insert_symbol(st, s);                                                   \
        printf("Identifier: %s was added\n", s->name);                          \
    } else {                                                                    \
        printf("Identifier: %s is trying to be re-declared\n", symbol_name);    \
        yyerror();                                                              \
    }                                                                           \
    s;                                                                          \
 })

#define SEARCH_SYMBOL(symbol_name) ({                                   \
    symbol_t* s = search_symbol(st, symbol_name);                       \
    if (s == NULL) {                                                    \
        printf("Error - Undeclared identifier: %s\n", symbol_name);     \
        yyerror();                                                      \
    } else {                                                            \
        printf("Identifier found: %s\n", s->name);                      \
    }                                                                   \
    s;                                                                  \
})

#define CREATE_EXPRESSION(symbol_name, left, right) ({                  \
    symbol_t* s = create_symbol();                                      \
    s->flag = OP_F;                                                     \
    s->name = symbol_name;                                              \
    tree_node_t* node = init_tree_s(s, left, right);                    \
    node;                                                               \
})

#define CREATE_CONST(symbol_type, symbol_value) ({            \
    symbol_t* s = create_symbol();                            \
    s->flag = BASIC_F;                                        \
    s->type = symbol_type;                                    \
    s->value = symbol_value;                                  \
    tree_node_t* node = init_leaf_s(s);                       \
    node;                                                     \
})

// FUNCTIONS

const char* extension(const char path[]);

int atob(char* bool);

#endif
