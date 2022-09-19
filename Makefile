OS := $(shell uname)

ifeq ($(OS), Darwin)
	CC = gcc-12
else
	CC = gcc
endif

# DEFINE FUNCTIONS

define files
	$(wildcard $(1)/*.c)
endef

define test
	$(wildcard $(subst main,test,$(1))/*test*.c)
endef

# SET PATHS

UTILS_PATH := src/main/utils
TYPECHECK_PATH := src/main/typecheck
BUILDER_PATH:= src/main/npc/builder
EVAL_PATH:= src/main/eval
LIST_PATH := src/main/list
STACK_PATH := src/main/stack
TREE_PATH := src/main/tree
SYMBOL_PATH := src/main/symbol
SYMBOL_LIST_PATH := src/main/symbol-list
SYMBOL_TABLE_PATH := src/main/symbol-table
SYNTAX_TREE_PATH := src/main/syntax-tree
INSTRUCTION_PATH := src/main/instruction
CODE_GEN_PATH := src/main/code-gen

LEXER_PATH := src/main/npc/lexer.l
PARSER_PATH := src/main/npc/parser.y

# FETCH FILES

UTILS :=  $(call files, $(UTILS_PATH))
TYPECHECK :=  $(call files, $(TYPECHECK_PATH))
EVAL :=  $(call files, $(EVAL_PATH))
BUILDER :=  $(call files, $(BUILDER_PATH))
LIST := $(call files, $(LIST_PATH))
STACK := $(call files, $(STACK_PATH))
TREE := $(call files, $(TREE_PATH))
SYMBOL := $(call files, $(SYMBOL_PATH))
SYMBOL_LIST := $(call files, $(SYMBOL_LIST_PATH))
SYMBOL_TABLE := $(call files, $(SYMBOL_TABLE_PATH))
SYNTAX_TREE := $(call files, $(SYNTAX_TREE_PATH))
INSTRUCTION := $(call files, $(INSTRUCTION_PATH))
CODE_GEN := $(call files, $(CODE_GEN_PATH))

# COMPILER INFRASTRUCTURE RULES

LEXER := src/main/npc/lex.yy.c
PARSER := src/main/npc/parser.tab.c src/main/npc/parser.tab.h
MAIN := src/main/npc/main.c

$(LEXER): $(LEXER_PATH)
	flex -o $@ $^

$(PARSER): $(PARSER_PATH)
	bison -d -o $@ $^

TARGETS := utils \
		   typecheck \
		   eval \
		   builder \
           list \
           stack \
           tree \
           symbol_list \
           symbol_table \
           syntax_tree \
		   instruction \
		   code_gen \
           npc

COMMON := $(LEXER) \
		  $(PARSER) \
		  $(UTILS) \
		  $(TYPECHECK) \
		  $(EVAL) \
		  $(BUILDER) \
		  $(SYNTAX_TREE) \
		  $(SYMBOL_TABLE) \
		  $(SYMBOL_LIST) \
		  $(SYMBOL) \
		  $(STACK) \
		  $(TREE) \
		  $(LIST) \
		  $(INSTRUCTION) \
		  $(CODE_GEN)

utils: $(COMMON) \
       $(call test, $(UTILS_PATH))

typecheck: $(COMMON) \
		   $(call test, $(TYPECHECK_PATH))

eval: $(COMMON) \
	  $(call test, $(EVAL_PATH))

builder: $(COMMON) \
		 $(call test, $(BUILDER_PATH))

list: $(LIST) \
      $(call test, $(LIST_PATH))

tree: $(TREE) \
      $(call test, $(TREE_PATH))

stack: $(STACK) \
       $(LIST) \
       $(call test, $(STACK_PATH))

symbol_list: $(SYMBOL_LIST) \
             $(LIST) \
             $(SYMBOL) \
             $(call test, $(SYMBOL_LIST_PATH))

symbol_table: $(SYMBOL_TABLE) \
              $(SYMBOL_LIST) \
              $(SYMBOL) \
              $(STACK) \
              $(LIST) \
              $(call test, $(SYMBOL_TABLE_PATH))

syntax_tree: $(SYNTAX_TREE) \
             $(SYMBOL) \
             $(TREE) \
             $(call test, $(SYNTAX_TREE_PATH))

instruction: $(INSTRUCTION) \
             $(LIST) \
             $(SYMBOL) \
			 $(call test, $(INSTRUCTION_PATH))

code_gen: $(CODE_GEN) \
	      $(INSTRUCTION) \
          $(LIST) \
          $(TREE) \
		  $(call test, $(CODE_GEN_PATH))

# COMPILER RULE

npc: $(COMMON) \
     $(MAIN)

# BUILD RULES

$(TARGETS):
	$(CC) -o $@ $^

all: $(TARGETS)

# PHONY RULES

.PHONY: clean

clean:
	rm -f $(LEXER) $(PARSER) $(TARGETS)
