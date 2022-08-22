%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%token INT
%token BOOL
%token ID

%type expr
%type CONST
    
%left '+' '-' 
%left '*'
%left '|'
%left '&'
 
%%
 
prog: expr ';'  { printf("No hay errores \n"); } 
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


