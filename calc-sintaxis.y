%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%token ID
%token TYPE
%token INT
%token BOOL
%token RETURN

%type expr
%type CONST
    
%left '+' '-' 
%left '*'
%left '|'
%left '&'
 
%%
 
prog: decl subprog

    | decl  { printf("No hay errores \n"); } 

    | RETURN expr ';'
    ;

subprog: decl subprog

       | assign subprog

       | RETURN expr ';' subprog

       |
       ;

decl: TYPE assign ';'
    ;

assign: ID '=' expr ';'
      ;
  
expr: CONST               

    | ID

    | expr '+' expr    
    
    | expr '*' expr

    | expr '-' expr  

    | expr '&' expr  

    | expr '|' expr  

    | '(' expr ')'      
    ;


CONST : INT              

    | BOOL
    ;
 
%%


