#include <string.h>
#include <limits.h>

#include "suggestor.h"

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

char* get_suggestion(char* word, list_t* poss_suggestions) {
    char* best_suggestion;
    int best_distance = INT_MAX;
    node_t* cursor = poss_suggestions->head->next;
    while (cursor) {
        char* new_suggestion = (char*) cursor->value;
        int new_distance = levenshtein(word, new_suggestion);
        if (new_distance < best_distance) {
            best_distance = new_distance;
            best_suggestion = new_suggestion;
        }
        cursor = cursor->next;
    }
    return best_suggestion;
}


