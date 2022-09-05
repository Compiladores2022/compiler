%{

#include <stdlib.h>
#include <stdio.h>

#include "../symbol-table/symbol-table.h"
#include "../symbol/symbol.h"
#include "../symbol/flags.h"
#include "../symbol/types.h"
#include "../syntax-tree/syntax-tree.h"
#include "../tree/tree-node.h"
#include "../utils/utils.h"

void yyerror();
int yylex();

symtable_t* st;

%}

%union {
    int ival;
    int bval;
    char* sval;
    struct tree_node_s* node;
}
 
%token <sval> ID
%token TYPE
%token <ival> INT
%token <bval> BOOL
%token RETURN

%type <node> expr
%type <node> CONST
    
%left <sval> '+' '-'
%left <sval> '*'
%left <sval> '|'
%left <sval> '&'
 
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
    TYPE ID '=' expr            { ADD_SYMBOL($2); }
    | TYPE ID                   { ADD_SYMBOL($2); }
    ;

assign: 
      ID '=' expr               { SEARCH_SYMBOL($1); }
      ;

expr:
    CONST                       { $$ = $1; }
    | ID                        { $$ = init_leaf_s(SEARCH_SYMBOL($1)); }
    | expr '+' expr             { $$ = CREATE_EXPRESSION($2, $1, $3); }
    | expr '-' expr             { $$ = CREATE_EXPRESSION($2, $1, $3); }
    | expr '*' expr             { $$ = CREATE_EXPRESSION($2, $1, $3); }
    | expr '|' expr             { $$ = CREATE_EXPRESSION($2, $1, $3); }
    | expr '&' expr             { $$ = CREATE_EXPRESSION($2, $1, $3); }
    | '(' expr ')'              { $$ = $2; }
    ;

CONST:
     INT                        { $$ = CREATE_CONST(INT_T, $1); }
     | BOOL                     { $$ = CREATE_CONST(BOOL_T, $1); }
     ;
 
%%
