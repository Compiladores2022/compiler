%{

#include <stdlib.h>
#include <stdio.h>
#include "symbol-table/symbol-table.h"
#include "symbol/symbol.h"

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
 



prog: 
    { 
        st = init_symtable();
        push_level(st);
    } decl ';' subprog { printf("No errors were found \n"); }
    | RETURN expr ';' { printf("No errors were found \n"); }
    ;

subprog: assign ';'
       | RETURN expr ';' 
       | decl ';' subprog
       | assign ';' subprog
       | RETURN expr ';' subprog
       ;

decl: TYPE ID 
    { 
        symbol_t* s = create_symbol();
        s->name = $2;
        insert_symbol(st, s);
        printf("Identifier: %s was added\n", s->name);
    } '=' expr
    | TYPE ID { 
        symbol_t* s = create_symbol();
        s->name = $2;
        insert_symbol(st, s);
        printf("Identifier: %s was added\n", s->name);
    }
    ;

assign: 
      ID { 
        symbol_t* s = search_symbol(st, $1);
        if (s == NULL) {
            printf("Error - Undeclared identifier: %s\n", $1);
            yyerror();
        } else {
            printf("Identifier found: %s\n", s->name);
        }
      } '=' expr 
      ;

expr: CONST               
    | ID 
    {
        symbol_t* s = search_symbol(st, $1);
        if (s == NULL) {
            printf("Error - Undeclared identifier: %s\n", $1);
            yyerror();
        } else {
            printf("Identifier found: %s\n", s->name);
        }
    }
    | expr '+' expr    
    | expr '-' expr  
    | expr '*' expr
    | expr '|' expr  
    | expr '&' expr  
    | '(' expr ')'      
    ;


CONST: INT              
     | BOOL
     ;
 
%%


