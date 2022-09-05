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
%token <ival> TYPE
%token <ival> INT
%token <bval> BOOL
%token RETURN

%type <node> decl
%type <node> assign
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
    TYPE ID '=' expr            {   
                                    symbol_t* symbol = ADD_SYMBOL($2, $1);
                                    symbol_t* s = create_symbol();
                                    s->flag = DECL_F;
                                    s->type = symbol->type;
                                    s->name = "=";
                                    printf("Creating declaration of '%s' of type %d\n", symbol->name, $1);
                                    $$ = init_tree_s(s, init_leaf_s(symbol), $4);
                                }
    | TYPE ID                   { 
                                    symbol_t* symbol = ADD_SYMBOL($2, $1);
                                    printf("Creating declaration of '%s' of type %d\n", symbol->name, $1);
                                    $$ = init_leaf_s(symbol); 
                                }
    ;

assign: 
      ID '=' expr               { symbol_t* s = SEARCH_SYMBOL($1); $$ = CREATE_ASSIGNMENT(s, $3); }
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
