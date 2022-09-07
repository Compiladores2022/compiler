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

init:                           { st = symbol_table(st); }
    prog                        { out_msg(0); }
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
                                    symbol_t* symbol = build_symbol(yyerror, st, $2, $1);
                                    $$ = add_declaration(symbol, init_leaf_s(symbol), $4);
                                }
    | TYPE ID                   { 
                                    symbol_t* symbol = build_symbol(yyerror, st, $2, $1);
                                    printf("Creating declaration of '%s' of type %d\n", symbol->name, $1);
                                    $$ = init_leaf_s(symbol); 
                                }
    ;

assign: 
ID '=' expr                     { symbol_t* s = search_symbol_p(yyerror, st, $1);
                                  $$ = create_assignment(s, $3);
                                }
      ;

expr:
    CONST                       { $$ = $1; }
    | ID                        { $$ = init_leaf_s(search_symbol_p(yyerror, st, $1)); }
    | expr '+' expr             { $$ = create_expression($2, $1, $3); }
    | expr '-' expr             { $$ = create_expression($2, $1, $3); }
    | expr '*' expr             { $$ = create_expression($2, $1, $3); }
    | expr '|' expr             { $$ = create_expression($2, $1, $3); }
    | expr '&' expr             { $$ = create_expression($2, $1, $3); }
    | '(' expr ')'              { $$ = $2; }
    ;

CONST:
     INT                        { $$ = create_const(INT_T, $1); }
     | BOOL                     { $$ = create_const(BOOL_T, $1); }
     ;
 
%%
