%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "parser.tab.h"
#include "../utils/utils.h"

void yyerror();

%}

%option noyywrap
%option yylineno

char [a-zA-Z]
lower_char [a-z]
number [0-9]+
bool "true"|"false"
type "int"|"bool"
return "return"
id {lower_char}({char}|{number})*
operator [&|+\-*;()=]
comment "//".*\n
ignore \n|" "
other .

%x C_COMMENT

%%

{number}                        {
                                   printf("INT : %d\n",atoi(yytext));
                                   yylval.ival = atoi(yytext);
                                   return INT;
                                }

{bool}                          {
                                   printf("BOOL : %s\n", yytext);
                                   yylval.bval = atob(yytext);
                                   return BOOL;
                                }

{type}                          {
                                   printf("TYPE : %s\n", yytext);
                                   return TYPE;
                                }

{return}                        {
                                   printf("RETURN : %s\n", yytext);
                                   return RETURN;
                                }

{id}                            {
                                   printf("ID : %s\n",yytext);
                                   yylval.sval = strdup(yytext);
                                   return ID;
                                }

{operator}                      {
                                   printf("%s\n",yytext);
                                   yylval.sval = strdup(yytext);
                                   return *yytext;
                                }

{comment}                        ; /* ignore inline comments*/

"/*"                             { BEGIN(C_COMMENT); }
<C_COMMENT>"*/"                  { BEGIN(INITIAL); }
<C_COMMENT>\n                    { }
<C_COMMENT>.                     { }

{ignore}                         ; /* ignore some special symbols */

{other}                          { yyerror(); }

%%

void yyerror(){
	printf("%s%d\n","-> Syntax error in line: ",yylineno);
    exit(1);
}

int main(int argc,char *argv[]){
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
