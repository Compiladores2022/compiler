#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"

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

int atob(char* bool) {

    if (!strcmp("false", bool))
        return 0;

    if (!strcmp("true", bool))
        return 1;

    printf("Invalid boolean value");
    exit(1);
}
