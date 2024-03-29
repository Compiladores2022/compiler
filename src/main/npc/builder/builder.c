#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../../utils/utils.h"
#include "builder.h"
#include "../parser.tab.h"

#define MEM_OFFSET 4

int glob_offset = -1;

symtable_t* symbol_table(symtable_t* st) {
    st = init_symtable();
    push_level(st);
    return st;
}

void out_msg(int status) {
    if (status) {
        printf("There were compile-time errors!\n");
    }
}

symbol_t* find_symbol(symtable_t* st, char* symbol_name) {
    symbol_t* s = search_symbol(st, symbol_name);
    if (s == NULL) {
        char* err_msg = malloc(50 * sizeof(char));
        sprintf(err_msg, "Undeclared identifier '%s'", symbol_name);
        yyerror(format_err(err_msg, lineno()));
    }
    return s;
}

symbol_t* build_id(symtable_t* st, char* symbol_name, type_t symbol_type, flag_t flag) {
    symbol_t* s = NULL;
    list_t* l = (list_t*) st->stack->head->next->value;
    if (search_symbol_l(l, symbol_name) != NULL) {
        char* msg = (char*) malloc(sizeof(char) * 100);
        sprintf(msg, "Identifier '%s' is trying to be re-declared", symbol_name);
        yyerror(format_err(msg, lineno()));
    }
    // The symbol is created anyway in order to continue the error checking to show all the errors
    s = create_symbol(); 
    s->name = symbol_name;
    s->flag = flag;
    s->type = symbol_type;
    s->value = 0; // Default value
    s->global = 0;
    if (flag == ID_F || flag == PARAM_F) {
        s->offset = (glob_offset--) * MEM_OFFSET;
    }
    insert_symbol(st, s);
    return s;
}

tree_node_t* build_const(type_t symbol_type, int symbol_value) {
    symbol_t* s = create_symbol();
    s->flag = BASIC_F;
    s->type = symbol_type;
    s->value = symbol_value;
    return init_leaf_s(s);
}

tree_node_t* build_unary_expr(char* symbol_name, tree_node_t* middle) {
    symbol_t* s = create_symbol();
    s->flag = UN_OP_F;
    s->name = symbol_name;
    s->lineno = lineno();
    s->offset = (glob_offset--) * MEM_OFFSET;
    return init_unary_tree_s(s, middle);
}

tree_node_t* build_binary_expr(char* symbol_name, tree_node_t* left, tree_node_t* right) {
    symbol_t* s = create_symbol();
    s->flag = BIN_OP_F;
    s->name = symbol_name;
    s->lineno = lineno();
    s->offset = (glob_offset--) * MEM_OFFSET;
    return init_binary_tree_s(s, left, right);
}

tree_node_t* build_assignment(symtable_t* st, char* symbol_name, tree_node_t* right) {
    symbol_t* symbol = find_symbol(st, symbol_name);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = ASSIGN_F;
    s->name = "=";
    s->lineno = lineno();
    return init_binary_tree_s(s, left, right);
}

tree_node_t* build_declaration(symtable_t* st, type_t symbol_type, char* symbol_name, tree_node_t* right) {
    symbol_t* symbol = build_id(st, symbol_name, symbol_type, ID_F);
    tree_node_t* left = init_leaf_s(symbol);
    symbol_t* s = create_symbol();
    s->flag = DECL_F;
    s->name = "=";
    s->lineno = lineno();
    return init_binary_tree_s(s, left, right);
}

tree_node_t* build_return(tree_node_t* expr) {
    symbol_t* s = create_symbol();
    s->flag = RETURN_F;
    s->name = "return";
    s->lineno = lineno();
    return init_unary_tree_s(s, expr);
}

tree_node_t* link(tree_node_t* left, tree_node_t* right) {
    symbol_t* s = create_symbol();
    s->flag = LINK_F;
    s->name = ";";
    return init_binary_tree_s(s, left, right);
}

tree_node_t* build_block(tree_node_t* declarations, tree_node_t* statements) {
    symbol_t* s = create_symbol();
    s->flag = BLOCK_F;
    s->name = "block";
    return init_binary_tree_s(s, declarations, statements);
}

tree_node_t* build_if(tree_node_t* expr, tree_node_t* then_block, tree_node_t* else_block) {
    symbol_t* s = create_symbol();
    s->flag = IF_F;
    s->name = "if";
    return init_ternary_tree_s(s, then_block, expr, else_block);
}

tree_node_t* build_while(tree_node_t* expr, tree_node_t* while_block) {
    symbol_t* s = create_symbol();
    s->flag = WHILE_F;
    s->name = "while";
    return init_binary_tree_s(s, expr, while_block);
}

tree_node_t* build_param(symtable_t* st, type_t param_type, char* param_name) {
    symbol_t* symbol = build_id(st, param_name, param_type, PARAM_F);
    return init_leaf_s(symbol);
}

tree_node_t* build_procedure_symbol(symtable_t* st, type_t proc_type, char* proc_name) {
    symbol_t* symbol = build_id(st, proc_name, proc_type, PROC_F);
    register_procedure(symbol);
    return init_leaf_s(symbol);
}

tree_node_t* build_procedure_params(tree_node_t* proc, tree_node_t* params) {
    symbol_t* symbol = (symbol_t*) proc->value;
    if (!strcmp(symbol->name, "main")) {
        validate_main_profile(symbol->type, params);
    }
    symbol->params = init_list();
    enlist(params, symbol->params);
    return init_leaf_s(symbol);
}

tree_node_t* build_procedure(tree_node_t* proc, tree_node_t* proc_block) {
    symbol_t* symbol = (symbol_t*) proc->value;
    return init_unary_tree_s(symbol, proc_block);
}

tree_node_t* build_call(symtable_t* st, char* proc_name, tree_node_t* arguments) {
    symbol_t* s = create_symbol();
    symbol_t* proc_symbol = find_symbol(st, proc_name);
    if (proc_symbol == NULL) {
        return NULL;
    }
    s->flag = CALL_F;
    s->name = proc_name;
    s->lineno = lineno();
    s->params = proc_symbol->params;
    s->type = proc_symbol->type;
    s->offset = (glob_offset--) * MEM_OFFSET;
    return init_unary_tree_s(s, arguments);
}

tree_node_t* build_prog(tree_node_t* declarations, tree_node_t* procedures) {
    symbol_t* s = create_symbol();
    s->flag = PROG_F;
    s->name = "program";
    return init_binary_tree_s(s, declarations, procedures);
}
