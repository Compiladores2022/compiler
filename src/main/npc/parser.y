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
%token PROG
%token EXTERN
%token ASSIGNMENT
%token EQT
%token RETURN

%type <node> program
%type <node> statement
%type <node> declaration
%type <node> assignment
%type <node> expr
%type <node> LITERAL
    
%left <sval> '||'
%left <sval> '&&'
%left <sval> EQT
%left <sval> '<' '>'
%left <sval> '+' '-'
%left <sval> '*' '/' '%'
%right <sval> '!' // I dont know if this is necessary, maybe %type <sval> '!' should work
 
%%

init:                               { st = symbol_table(st); }
    program                         { 
                                        root = $2; traverse_tree(root, check_types); 
                                        instruction_seq = new_instruction_seq();
                                        traverse_tree(root, build_instruction_seq);
                                        show_list(instruction_seq);
                                        create_asm(asm_filename(filename), instruction_seq);
                                        out_msg(0); 
                                    }
    ;

program:
       RETURN expr ';'              { $$ = build_return($2); }
       | statement ';'              { $$ = $1; }
       | statement ';' program      { $$ = link_statements($1, $3); }
       ;

statement:
         assignment                 { $$ = $1; }
         | declaration              { $$ = $1; }

declaration:
           TYPE ID ASSIGNMENT expr         { $$ = build_declaration(st, $2, $1, $4); }
           | TYPE ID                { $$ = build_declaration(st, $2, $1, NULL); }
           ;

assignment: 
      ID ASSIGNMENT expr                   { $$ = build_assignment(st, $1, $3); }
      ;

expr:
    LITERAL                         { $$ = $1; }
    | ID                            { $$ = init_leaf_s(find_symbol(st, $1)); }
    | expr '||' expr                { $$ = build_binary_expr($2, $1, $3); }
    | expr '&&' expr                { $$ = build_binary_expr($2, $1, $3); }
    | expr EQT expr                 { $$ = build_binary_expr($2, $1, $3); }
    | expr '<' expr                 { $$ = build_binary_expr($2, $1, $3); }
    | expr '>' expr                 { $$ = build_binary_expr($2, $1, $3); }
    | expr '+' expr                 { $$ = build_binary_expr($2, $1, $3); }
    | expr '-' expr                 { $$ = build_binary_expr($2, $1, $3); }
    | expr '*' expr                 { $$ = build_binary_expr($2, $1, $3); }
    | expr '/' expr                 { $$ = build_binary_expr($2, $1, $3); }
    | expr '%' expr                 { $$ = build_binary_expr($2, $1, $3); }
    | '!' expr                      { $$ = build_unary_expr($1, $2); }
    | '-' expr %prec '!'            { $$ = build_unary_expr($1, $2); }        
    | '(' expr ')'                  { $$ = $2; }
    ;

LITERAL:
       INTEGER                        { $$ = build_const(INT_T, $1); }
       | BOOL                         { $$ = build_const(BOOL_T, $1); }
       ;
 
%%
