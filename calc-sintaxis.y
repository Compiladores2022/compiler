%{

#include <stdlib.h>
#include <stdio.h>

%}
 
%token INT
%token ID

%type expr
%type VALOR
    
%left '+' '-' 
%left '*'
%left '|'
%left '&'
 
%%
 
prog: expr ';'  { printf("No hay errores \n"); } 
    ;
  
expr: VALOR               

    | expr '+' expr    
    
    | expr '*' expr

    | expr '-' expr  

    | expr '&' expr  

    | expr '|' expr  

    | '(' expr ')'      
    ;


VALOR : INT              

    | TRUE

    | FALSE
    ;
 
%%


