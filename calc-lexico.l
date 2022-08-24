%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "calc-sintaxis.tab.h"

%}

%option noyywrap
%option yylineno

char [a-zA-Z]
digit [0-9]
bool "true"|"false"
type "int"|"bool"
return "return"

%x C_COMMENT

%%

{digit}+                     {
                                printf("INT : %d\n",atoi(yytext));
                                return INT;}

{bool}                       {
                                printf("BOOL : %s\n", yytext);
                                return BOOL;}

{type}                       {
                                printf("TYPE : %s\n", yytext);
                                return TYPE;}

{return}                     {
                                printf("RETURN : %s\n", yytext);
                                return RETURN;}

{char}({char}|{digit})*      {
                                printf("ID : %s\n",yytext);
                                return ID;}

[&|+\-*;()=]                 {  
                                printf("%s\n",yytext);
                                return *yytext;}

.|\n|"//".*"\n"              ; /* ignore all the rest */

"/*"            { BEGIN(C_COMMENT); }
<C_COMMENT>"*/" { BEGIN(INITIAL); }
<C_COMMENT>\n   { }
<C_COMMENT>.    { }

%%

void yyerror(){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	yyparse();

}
