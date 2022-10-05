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

alpha [a-zA-Z]
digit [0-9]
alpha_num {alpha}|{digit}|_
bool_literal    "true"|"false"
integer_literal {digit}+
type "integer"|"bool"
void "void"
id {alpha}{alpha_num}*
arith_op [+\-*/%]
or "||"
and "&&"
neg "!"
relat_op [<>]
eq_op "=="
bin_op {arith_op}|{neg}|{relat_op}
assignment "="
parenthesis [()]
brackets [\{\}]
eol ";"
comma ","
program "program"
extern "extern"
if "if"
then "then"
else "else"
while "while"
return "return"
comment "//".*\n
ignore \n|" "
other .

%x C_COMMENT

%%

{eol}                           {
                                    printf("EOL : %s\n", yytext);
                                    return *yytext;
                                }

{comma}                         {
                                    printf("COMMA : %s\n", yytext);
                                    return *yytext;
                                }

{brackets}                      {
                                    printf("BRACKET : %s\n", yytext);
                                    return *yytext;
                                }

{program}                       {
                                    printf("PROG : %s\n",yytext);
                                    return PROG;
                                }

{extern}                        {
                                    printf("EXTERN : %s\n",yytext);
                                    return EXTERN;
                                }

{integer_literal}               {
                                    printf("INTEGER : %d\n",atoi(yytext));
                                    yylval.ival = atoi(yytext);
                                    return INTEGER;
                                }

{bool_literal}                  {
                                    printf("BOOL : %s\n", yytext);
                                    yylval.bval = atob(yytext);
                                    return BOOL;
                                }

{type}                          {
                                    printf("TYPE : %s\n", yytext);
                                    yylval.tval = atot(yytext);
                                    return TYPE;
                                }

{void}                          {
                                    printf("VOID : %s\n", yytext);
                                    return VOID;
                                }

{return}                        {
                                    printf("RETURN : %s\n", yytext);
                                    return RETURN;
                                }

{assignment}                    {
                                    printf("ASSIGNMENT : %s\n", yytext);
                                    yylval.sval = strdup(yytext);
                                    return ASSIGNMENT;
                                }

{parenthesis}                   {
                                    return *yytext;
                                }

{eq_op}                         {
                                    printf("BIN_OP : %s\n", yytext);
                                    yylval.sval = strdup(yytext);
                                    return EQT;

                                }

{or}                            {
                                    yylval.sval = strdup(yytext);
                                    return ORT;
                                }

{and}                            {
                                    yylval.sval = strdup(yytext);
                                    return ANDT;
                                }

{bin_op}                        {
                                    printf("OP : %s\n",yytext);
                                    yylval.sval = strdup(yytext);
                                    return *yytext;
                                }

{if}                            {
                                    printf("IF : %s\n",yytext);
                                    return IF;
                                }

{then}                          {
                                    printf("THEN : %s\n",yytext);
                                    return THEN;
                                }

{else}                          {
                                    printf("ELSE : %s\n",yytext);
                                    return ELSE;
                                }

{while}                         {
                                    printf("WHILE : %s\n",yytext);
                                    return WHILE;
                                }

{id}                            {
                                    printf("ID : %s\n",yytext);
                                    yylval.sval = strdup(yytext);
                                    return ID;
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
