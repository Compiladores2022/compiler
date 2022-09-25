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

%type <node> program
%type <node> statement
%type <node> declaration
%type <node> assignment
%type <node> expr
%type <node> CONST
    
%left <sval> '+' '-'
%left <sval> '*'
%left <sval> '|'
%left <sval> '&'
 
%%

init:                               { st = symbol_table(st); }
    program                         { 
                                        root = $2; traverse_tree(root, check_types); traverse_tree(root, evaluate); 
                                        instruction_seq = new_instruction_seq();
                                        traverse_tree(root, build_instruction_seq);
                                        show_list(instruction_seq);
                                        create_asm("prog.s", instruction_seq);
                                        out_msg(0); 
                                    }
    ;

program:
       statement ';'                { $$ = $1; }
       | declaration ';' program    { $$ = link_statements($1, $3); }
       | statement ';' program      { $$ = link_statements($1, $3); }
       ;

statement:
         assignment                 { $$ = $1; }
         | RETURN expr              { $$ = build_return($2); }

declaration:
           TYPE ID '=' expr         { $$ = build_declaration(st, $2, $1, $4); }
           | TYPE ID                { $$ = build_declaration(st, $2, $1, NULL); }
           ;

assignment: 
      ID '=' expr                   { $$ = build_assignment(st, $1, $3); }
      ;

expr:
    CONST                           { $$ = $1; }
    | ID                            { $$ = init_leaf_s(find_symbol(st, $1)); }
    | expr '+' expr                 { $$ = build_expression($2, $1, $3); }
    | expr '-' expr                 { $$ = build_expression($2, $1, $3); }
    | expr '*' expr                 { $$ = build_expression($2, $1, $3); }
    | expr '|' expr                 { $$ = build_expression($2, $1, $3); }
    | expr '&' expr                 { $$ = build_expression($2, $1, $3); }
    | '(' expr ')'                  { $$ = $2; }
    ;

CONST:
     INT                            { $$ = build_const(INT_T, $1); }
     | BOOL                         { $$ = build_const(BOOL_T, $1); }
     ;
 
%%
