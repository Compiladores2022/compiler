%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%token ID
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
 
prog: decl ';' subprog { printf("No errors were found \n"); }
    | RETURN expr ';' { printf("No errors were found \n"); }
    ;

subprog: /* empty */ 
       | decl ';' subprog
       | assign ';' subprog
       | RETURN expr ';' subprog
       ;

decl: TYPE assign
    | TYPE ID
    ;

assign: ID '=' expr
      ;

expr: CONST               
    | ID
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


