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

// FUNCTIONS

const char* extension(const char path[]);

#endif
