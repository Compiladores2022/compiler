%{

#include <stdlib.h>
#include <stdio.h>

#include "../symbol-table/symbol-table.h"
#include "../symbol/symbol.h"
#include "../symbol/flags.h"
#include "../symbol/types.h"
#include "../syntax-tree/syntax-tree.h"
#include "../tree/tree-node.h"
#include "../list/list.h"
#include "../instruction/instruction.h"
#include "../instruction/instruction-sequence.h"
#include "../utils/utils.h"
#include "../typecheck/typecheck.h"
#include "../eval/eval.h"
#include "../code-gen/instr-seq-gen.h"
#include "../code-gen/asm-gen.h"
#include "builder/builder.h"

void yyerror(const char* msg);
int yylex();

symtable_t* st;
tree_node_t* root;
extern list_t* instruction_seq;
extern char* filename;

%}

%union {
    int ival;
    int bval;
    char* sval;
    int tval;
    struct tree_node_s* node;
}
 
%token <tval> TYPE
%token <ival> INTEGER
%token <bval> BOOL
%token <sval> ID
%token VOID
%token PROG
%token EXTERN
%token ASSIGNMENT
%token EQT
%token RETURN
%token IF
%token THEN
%token ELSE
%token WHILE


/* %type <node> program */
/* %type <node> statement */
/* %type <node> declaration */
/* %type <node> assignment */
/* %type <node> expr */
/* %type <node> LITERAL */
    
%left <sval> ORT
%left <sval> ANDT
%left <sval> EQT
%left <sval> '<' '>'
%left <sval> '+' '-'
%left <sval> '*' '/' '%'
%right <sval> '!' // I dont know if this is necessary, maybe %type <sval> '!' should work
 
%%

/* init:                               { st = symbol_table(st); } */
/*     program                         {  */
/*                                         root = $2; traverse_tree(root, check_types);  */
/*                                         instruction_seq = new_instruction_seq(); */
/*                                         traverse_tree(root, build_instruction_seq); */
/*                                         show_list(instruction_seq); */
/*                                         create_asm(asm_filename(filename), instruction_seq); */
/*                                         out_msg(0);  */
/*                                     } */
/*     ; */

program:
       PROG '{' '}'
       | PROG '{' methods '}'
       | PROG '{' declarations methods '}'
       ;

methods:
       method
       | methods method
       ;

method:
      TYPE ID '(' params ')' block
      | VOID ID '(' params ')' block
      | TYPE ID '(' params ')' EXTERN ';'
      | VOID ID '(' params ')' EXTERN ';'
      | TYPE ID '(' ')' block
      | VOID ID '(' ')' block
      | TYPE ID '(' ')' EXTERN ';'
      | VOID ID '(' ')' EXTERN ';'
      ;

params:
      TYPE ID
      | params ',' TYPE ID
      ;

statements:
          statement
          | statements statement
          ;

declarations:
            declaration
            | declarations declaration
            ;

statement:
         assignment                 //{ $$ = $1; }
         | method_call
         | conditional
         | while
         | RETURN expr ';'
         | RETURN ';'
         | block
         ;

conditional:
           IF '(' expr ')' THEN block
           | IF '(' expr ')' THEN block ELSE block
           ;

while:
     WHILE '(' expr ')' block
     ;

declaration:
           TYPE ID ASSIGNMENT expr ';'        //{ $$ = build_declaration(st, $2, $1, $4); }
           | TYPE ID ';'                //{ $$ = build_declaration(st, $2, $1, NULL); }
           ;

assignment: 
          ID ASSIGNMENT expr ';'                   //{ $$ = build_assignment(st, $1, $3); }
          ;

method_call:
           ID '(' ')'
           | ID '(' exprs ')'
           ;

block:
     '{' '}'
     | '{' statements '}'
     | '{' declarations statements '}'
     ;

exprs:
     expr
     | exprs ',' expr
     ;

expr:
    LITERAL                         //{ $$ = $1; }
    | ID                            //{ $$ = init_leaf_s(find_symbol(st, $1)); }
    | method_call
    | expr ORT expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr ANDT expr                //{ $$ = build_binary_expr($2, $1, $3); }
    | expr EQT expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr '<' expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr '>' expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr '+' expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr '-' expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr '*' expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr '/' expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | expr '%' expr                 //{ $$ = build_binary_expr($2, $1, $3); }
    | '!' expr                      //{ $$ = build_unary_expr($1, $2); }
    | '-' expr %prec '!'            //{ $$ = build_unary_expr($1, $2); }
    | '(' expr ')'                  //{ $$ = $2; }
    ;

LITERAL:
       INTEGER                        //{ $$ = build_const(INT_T, $1); }
       | BOOL                         //{ $$ = build_const(BOOL_T, $1); }
       ;
 
%%
