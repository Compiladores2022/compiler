%{

#include <stdlib.h>
#include <stdio.h>

#include "../symbol-table/symbol-table.h"
#include "../symbol/symbol.h"
#include "../utils/utils.h"

void yyerror();
int yylex();

symtable_t* st;

%}

%union {
    char* sval;
}
 
%token <sval> ID
%token TYPE
%token INT
%token BOOL
%token RETURN

//%type expr
//%type CONST
    
%left '+' '-'
%left '*'
%left '|'
%left '&'
 
%%

init:                           { SYMBOL_TABLE() }
    prog                        { NO_ERRORS() }
    ;

prog:
    assign ';'
    | RETURN expr ';' 
    | decl ';' prog
    | assign ';' prog
    | RETURN expr ';' prog
    ;

decl:
    TYPE ID '=' expr            { ADD_SYMBOL($2) }
    | TYPE ID                   { ADD_SYMBOL($2) }
    ;

assign: 
      ID '=' expr               { SEARCH_SYMBOL($1) }
      ;

expr:
    CONST
    | ID                        { SEARCH_SYMBOL($1) }
    | expr '+' expr    
    | expr '-' expr  
    | expr '*' expr
    | expr '|' expr  
    | expr '&' expr  
    | '(' expr ')'      
    ;

CONST:
     INT
     | BOOL
     ;
 
%%
