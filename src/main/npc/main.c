#include <stdio.h>
#include <string.h>

#include "parser.tab.h"
#include "../utils/utils.h"

extern FILE *yyin;
extern void yyerror(const char* msg);
extern int lineno();

int main(int argc,char *argv[]) {
	++argv,--argc;
	if (argc > 0) {
        if (strcmp(extension(argv[0]), ".np")) {
             printf("File extension not compatible with compiler\n");
             return 1;
        }
		yyin = fopen(argv[0],"r");
	} else {
		yyin = stdin;
    }
	yyparse();
    return 0;
}
