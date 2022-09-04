#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../../utils/utils.h"

int main() {
    assert(strcmp(extension("foo/bar.baz.qux"), ".qux") == 0);
    assert(strcmp(extension("foo.bar.baz/qux"), "") == 0);
    printf("Ok!\n");
    return 0;
}
