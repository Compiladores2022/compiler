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
tree_node_t* root;

%}

%union {
    int ival;
    int bval;
    char* sval;
    int tval;
    struct tree_node_s* node;
}
 
%token <sval> ID
%token <tval> TYPE
%token <ival> INT
%token <bval> BOOL
%token RETURN

%type <node> prog
%type <node> statement
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
    prog                        { root = $2; show_tree(root); out_msg(0); }
    ;

prog:
    statement ';'               { $$ = $1; }
    | decl ';' prog             { $$ = link_statements($1, $3); }
    | statement ';' prog        { $$ = link_statements($1, $3); }
    ;

statement:
         assign                 { $$ = $1; }
         | RETURN expr          { $$ = build_return($2); }

decl:
    TYPE ID '=' expr            { $$ = build_declaration(yyerror, st, $2, $1, $4); }
    | TYPE ID                   { $$ = build_declaration(yyerror, st, $2, $1, NULL); }
    ;

assign: 
      ID '=' expr               { $$ = build_assignment(yyerror, st, $1, $3); }
      ;

expr:
    CONST                       { $$ = $1; }
    | ID                        { $$ = init_leaf_s(find_symbol(yyerror, st, $1)); }
    | expr '+' expr             { $$ = build_expression($2, $1, $3); }
    | expr '-' expr             { $$ = build_expression($2, $1, $3); }
    | expr '*' expr             { $$ = build_expression($2, $1, $3); }
    | expr '|' expr             { $$ = build_expression($2, $1, $3); }
    | expr '&' expr             { $$ = build_expression($2, $1, $3); }
    | '(' expr ')'              { $$ = $2; }
    ;

CONST:
     INT                        { $$ = build_const(INT_T, $1); }
     | BOOL                     { $$ = build_const(BOOL_T, $1); }
     ;
 
%%
