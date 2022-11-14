#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "../npc/parser.tab.h"
#include "utils.h"

#include "suggestor.h"

list_t* symbols;

int min(int a, int b, int c) {
    if (a <= b && a <= c) {
        return a;
    } else if (b <= a && b <= c) {
        return b;
    } else if (c <= a && c <= b) {
        return c;
    }
}

int levenshtein(char *s1, char *s2) {
    unsigned int x, y, s1len, s2len;
    s1len = strlen(s1);
    s2len = strlen(s2);
    unsigned int matrix[s2len+1][s1len+1];
    matrix[0][0] = 0;
    for (x = 1; x <= s2len; x++) {
        matrix[x][0] = matrix[x-1][0] + 1;
    }

    for (y = 1; y <= s1len; y++) {
        matrix[0][y] = matrix[0][y-1] + 1;
    }

    for (x = 1; x <= s2len; x++) {
        for (y = 1; y <= s1len; y++) {
            matrix[x][y] = min(matrix[x-1][y] + 1, matrix[x][y-1] + 1, matrix[x-1][y-1] + (s1[y-1] == s2[x-1] ? 0 : 1));
        }
    }

    return(matrix[s2len][s1len]);
}

void load_symbols(list_t* symbols) {
    add(symbols, "true");
    add(symbols, "false");
    add(symbols, "void");
    add(symbols, "bool");
    add(symbols, "integer");
    add(symbols, "program");
    add(symbols, "extern");
    add(symbols, "if");
    add(symbols, "then");
    add(symbols, "else");
    add(symbols, "while");
    add(symbols, "return");
}

char* get_suggestion(char* word) {
    symbols = init_list();
    load_symbols(symbols);
    char* best_suggestion = malloc(15);
    int best_distance = INT_MAX;
    node_t* cursor = symbols->head->next;
    while (cursor) {
        char* new_suggestion = (char*) cursor->value;
        int new_distance = levenshtein(word, new_suggestion);
        if (new_distance < best_distance) {
            best_distance = new_distance;
            strcpy(best_suggestion, new_suggestion);
        }
        cursor = cursor->next;
    }

    if (best_distance >= (int) strlen(word)) {
        strcpy(best_suggestion, "");
    }

    // a valid symbol is being used as id (e.g., integer bool = 2)
    if (best_distance == 0) {
        char* err_msg = malloc(50 * sizeof(char));
        sprintf(err_msg, "Cannot use reserved word '%s' as an identifier.", word);
        yyerror(format_err(err_msg, lineno()));
        exit(1);
    }

    return best_suggestion;
}
