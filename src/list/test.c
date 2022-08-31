#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main() {
    list_t* l = init_list();

    for (int i = 0; i < 10; i++) {
        int* n = (int*)malloc(sizeof(int));
        *n = i;
        printf("Adding %d\n", i);
        add(l, (void*)n);
    }

    printf("Iterating list: \n");
    node_t* cursor = (l->head)->next;
    while (cursor != NULL) {
        int* v = (int*) cursor->value;
        printf("%d\n", *v);
        cursor = cursor->next;
    }

    return 0;
}
