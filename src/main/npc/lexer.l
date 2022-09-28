%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../utils/utils.h"
#include "../symbol/types.h"
#include "parser.tab.h"

void yyerror(const char* msg);

int lineno();

%}

%option noyywrap
%option yylineno

char [a-zA-Z]
lower_char [a-z]
number [0-9]+
bool "true"|"false"
type "integer"|"bool"
extern "extern"
program "program"
if "if"
then "then"
else "else"
while "while"
return "return"
id {lower_char}({char}|{number})*
operator [&|+\-*;()=!]
comment "//".*\n
ignore \n|" "
other .

%x C_COMMENT

%%

{program}                       {
                                   // printf("INT : %d\n",atoi(yytext));
                                   return PROG;
                                }

{extern}                        {
                                   // printf("INT : %d\n",atoi(yytext));
                                   return EXTERN;
                                }

{number}                        {
                                   // printf("INT : %d\n",atoi(yytext));
                                   yylval.ival = atoi(yytext);
                                   return INTEGER;
                                }

{bool}                          {
                                   // printf("BOOL : %s\n", yytext);
                                   yylval.bval = atob(yytext);
                                   return BOOL;
                                }

{type}                          {
                                   // printf("TYPE : %s\n", yytext);
                                   yylval.tval = atot(yytext);
                                   return TYPE;
                                }

{return}                        {
                                   // printf("RETURN : %s\n", yytext);
                                   return RETURN;
                                }

{id}                            {
                                   // printf("ID : %s\n",yytext);
                                   yylval.sval = strdup(yytext);
                                   return ID;
                                }

{operator}                      {
                                   // printf("%s\n",yytext);
                                   yylval.sval = strdup(yytext);
                                   return *yytext;
                                }

{comment}                        ; /* ignore inline comments*/

"/*"                             { BEGIN(C_COMMENT); }
<C_COMMENT>"*/"                  { BEGIN(INITIAL); }
<C_COMMENT>\n                    { }
<C_COMMENT>.                     { }

{ignore}                         ; /* ignore some special symbols */

{other}                          { yyerror(NULL); }

%%

void yyerror(const char* msg) {
    if (!msg) {
	    printf("%s%d\n","-> Syntax error in line: ",yylineno);
    } else {
        printf("%s\n", msg);
    }
    exit(1);
}

int lineno() {
    return yylineno;
}
