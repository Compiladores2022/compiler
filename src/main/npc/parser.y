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
list_t* procedures;
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


%type <node> program 
%type <node> statement
%type <node> statements
%type <node> declaration
%type <node> declarations
%type <node> assignment
%type <node> expr
%type <node> exprs
%type <node> procedure
%type <node> proc_decl
%type <node> proc_params_decl
%type <node> proc_no_params_decl
%type <node> procedures
%type <node> procedure_call
%type <node> while
%type <node> conditional
%type <node> return
%type <node> params
%type <node> block
%type <node> LITERAL

%left <sval> ORT
%left <sval> ANDT
%left <sval> EQT
%left <sval> '<' '>'
%left <sval> '+' '-'
%left <sval> '*' '/' '%'
%right <sval> '!' // I dont know if this is necessary, maybe %type <sval> '!' should work
 
%%

init:                               { st = symbol_table(st); procedures = init_list(); }
    program                         {
                                        root = $2; check_main(procedures);
                                        traverse_tree(root, check_types, 0);
                                        instruction_seq = new_instruction_seq();
                                        traverse_tree(root, build_instruction_seq, 1);
                                        create_asm(asm_filename(filename), instruction_seq);
                                        out_msg(0);
                                    }
    ;

program:
       PROG '{' '}'                             { $$ = build_prog(NULL, NULL); }
       | PROG '{' procedures '}'                { $$ = build_prog(NULL, $3); }
       | PROG '{' declarations procedures '}'   { $$ = build_prog($3, $4); }
       ;

procedures:
       procedure                                { $$ = $1; }
       | procedures procedure                   { $$ = link($1, $2); }
       ;

procedure:
      proc_params_decl block                    { pop_level(st); $$ = build_procedure($1, $2); }
      | proc_params_decl EXTERN ';'             { pop_level(st); $$ = build_procedure($1, NULL); }
      | proc_no_params_decl block               { $$ = build_procedure($1, $2); }
      | proc_no_params_decl EXTERN ';'          { $$ = build_procedure($1, NULL); }
      ;

proc_params_decl:
      proc_decl '(' params ')'                  { $$ = build_procedure_params($1, $3); } 
      ;

proc_no_params_decl:
      proc_decl '(' ')'                         { $$ = build_procedure_params($1, NULL); } 
      ;

proc_decl:
      TYPE ID                                   { $$ = build_procedure_symbol(st, $1, $2); }
      ; 

params: { push_level(st); }
      TYPE ID                                   { $$ = build_param(st, $2, $3); }
      | params ',' TYPE ID                      { $$ = link($1, build_param(st, $3, $4)); }
      ;


declarations:
            declaration                     { $$ = $1; }
            | declarations declaration      { $$ = link($1, $2); }
            ;

declaration:
           TYPE ID ASSIGNMENT expr ';'      { $$ = build_declaration(st, $1, $2, $4); }
           | TYPE ID ';'                    { $$ = build_declaration(st, $1, $2, NULL); }
           ;

statements:
          statement                         { $$ = $1; }
          | statements statement            { $$ = link($1, $2); }
          ;

statement:
         assignment                         { $$ = $1; }
         | procedure_call ';'               { $$ = $1; }
         | conditional                      { $$ = $1; }
         | while                            { $$ = $1; }
         | block                            { $$ = $1; }
         | return                           { $$ = $1; }
         ;

return:
      RETURN expr ';'                       { $$ = build_return($2); }
      | RETURN ';'                          { $$ = build_return(NULL); }
      ;

conditional:
           IF '(' expr ')' THEN block                                    { $$ = build_if($3, $6, NULL); }
           | IF '(' expr ')' THEN block ELSE block                       { $$ = build_if($3, $6, $8); }
           ;

while:
     WHILE '(' expr ')' block      { $$ = build_while($3, $5); }
     ;


assignment: 
          ID ASSIGNMENT expr ';'            { $$ = build_assignment(st, $1, $3); }
          ;

procedure_call:
           ID '(' ')'                              { $$ = build_call(st, $1, NULL); }
           | ID '(' exprs ')'                      { $$ = build_call(st, $1, $3); }
           ;

block:
     '{' '}'                                                    { $$ = build_block(NULL, NULL); }
     | '{' statements '}'                                       { $$ = build_block(NULL, $2); }
     | '{' { push_level(st); } declarations statements '}'      { $$ = build_block($3, $4); pop_level(st); }
     ;

exprs:
     expr                                   { $$ = $1; }
     | exprs ',' expr                       { $$ = link($1, $3); }
     ;

expr:
    LITERAL                                 { $$ = $1; }
    | ID                                    { $$ = init_leaf_s(find_symbol(st, $1)); }
    | procedure_call                        { $$ = $1; }
    | expr ORT expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr ANDT expr                        { $$ = build_binary_expr($2, $1, $3); }
    | expr EQT expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr '<' expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr '>' expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr '+' expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr '-' expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr '*' expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr '/' expr                         { $$ = build_binary_expr($2, $1, $3); }
    | expr '%' expr                         { $$ = build_binary_expr($2, $1, $3); }
    | '!' expr                              { $$ = build_unary_expr($1, $2); }
    | '-' expr %prec '!'                    { $$ = build_unary_expr($1, $2); }
    | '(' expr ')'                          { $$ = $2; }
    ;

LITERAL:
       INTEGER                              { $$ = build_const(INT_T, $1); }
       | BOOL                               { $$ = build_const(BOOL_T, $1); }
       ;
 
%%
