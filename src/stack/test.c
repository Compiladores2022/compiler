#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main() {
    stack s = initStack();

    for (int i = 0; i < 10; i++) {
        int* x = (int*)malloc(sizeof(int));
        *x = i;
        push(s, x);
        printf("pushed: %d\n", *((int*)top(s)));
    }

    for (int i = 0; i < 10; i++) {
        printf("top: %d\n", *((int*)top(s)));
        printf("poping: %d\n", *((int*)pop(s)));
    }

    return 0;
}
