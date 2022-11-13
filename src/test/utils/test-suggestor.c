#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "../../main/utils/suggestor.h"
#include "../../main/list/list.h"

int main() {
    list_t* list = init_list();
    char* integer_n = "integer";
    char* bool_n = "bool";
    char* void_n = "void";
    add(list, integer_n);
    add(list, bool_n);
    add(list, void_n);
    assert(strcmp(get_suggestion("int", list), "integer") == 0);
    assert(strcmp(get_suggestion("boolean", list), "bool") == 0);
    printf("Ok!\n");
    return 0;
}
