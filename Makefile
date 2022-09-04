CC = gcc

# DEFINE FUNCTIONS

define files
	$(filter-out $(1)/test.c, $(wildcard $(1)/*.c))
endef

define test
	$(wildcard $(1)/*test*.c)
endef

# SET PATHS

UTILS_PATH := src/utils
LIST_PATH := src/libs/list
STACK_PATH := src/libs/stack
TREE_PATH := src/libs/tree
SYMBOL_PATH := src/symbol
SYMBOL_LIST_PATH := src/symbol-list
SYMBOL_TABLE_PATH := src/symbol-table
SYNTAX_TREE_PATH := src/syntax-tree

LEXER_PATH := src/lexer.l
PARSER_PATH := src/parser.y

# FETCH FILES

UTILS :=  $(call files, $(UTILS_PATH))
LIST := $(call files, $(LIST_PATH))
STACK := $(call files, $(STACK_PATH))
TREE := $(call files, $(TREE_PATH))
SYMBOL := $(call files, $(SYMBOL_PATH))
SYMBOL_LIST := $(call files, $(SYMBOL_LIST_PATH))
SYMBOL_TABLE := $(call files, $(SYMBOL_TABLE_PATH))
SYNTAX_TREE := $(call files, $(SYNTAX_TREE_PATH))

# COMPILER INFRASTRUCTURE RULES

TARGETS := utils \
           list \
           stack \
           tree \
           symbol_list \
           symbol_table \
           syntax_tree \
           npc

utils: $(UTILS) $(call test, $(UTILS_PATH))
list: $(LIST) $(call test, $(LIST_PATH))
tree: $(TREE) $(call test, $(TREE_PATH))
stack: $(STACK) $(LIST) $(call test, $(STACK_PATH))
symbol_list: $(SYMBOL_LIST) $(LIST) $(SYMBOL) $(call test, $(SYMBOL_LIST_PATH))
symbol_table: $(SYMBOL_TABLE) $(SYMBOL_LIST) $(SYMBOL) $(STACK) $(LIST) $(call test, $(SYMBOL_TABLE_PATH))
syntax_tree: $(SYNTAX_TREE) $(SYMBOL) $(TREE) $(call test, $(SYNTAX_TREE_PATH))

# COMPILER RULES

LEXER := src/lex.yy.c
PARSER := src/parser.tab.c src/parser.tab.h

$(LEXER): $(LEXER_PATH)
	flex -o $@ $^

$(PARSER): $(PARSER_PATH)
	bison -d -o $@ $^

npc: $(LEXER) \
     $(PARSER) \
     $(SYNTAX_TREE) \
     $(SYMBOL_TABLE) \
     $(SYMBOL_LIST) \
     $(SYMBOL) \
     $(STACK) \
     $(TREE) \
     $(LIST) \
     $(UTILS)

# BUILD RULES

$(TARGETS):
	$(CC) -o $@ $^

all: $(TARGETS)

# PHONY RULES

.PHONY: clean

clean:
	rm -f $(LEXER) $(PARSER) $(TARGETS)
