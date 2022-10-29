#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.tab.h"
#include "../utils/utils.h"

extern FILE *yyin;
extern void yyerror(const char* msg);
extern int yyparse();
extern int lineno();
extern int error_count;

char* filename;

int main(int argc,char *argv[]) {
    error_count = 0;
    ++argv,--argc;
    if (argc > 0) {
        filename = argv[0];
        if (strcmp(extension(filename), ".np")) {
             printf("File extension not compatible with compiler\n");
             return 1;
        }
    yyin = fopen(argv[0],"r");
    } else {
        yyin = stdin;
    }
    yyparse();
    printf("%d errors found. \n", error_count);
    if (error_count > 0)
        exit(1);
    return 0;
}
