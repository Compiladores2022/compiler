CC = gcc

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

UTILS_TEST := $(wildcard $(UTILS_PATH)/*.c)
UTILS := $(filter-out $(UTILS_PATH)/test.c, $(UTILS_TEST))

LIST_TEST := $(wildcard $(LIST_PATH)/*.c)
LIST := $(filter-out $(LIST_PATH)/test.c, $(LIST_TEST))

STACK_TEST := $(wildcard $(STACK_PATH)/*.c)
STACK := $(filter-out $(STACK_PATH)/test.c, $(STACK_TEST))

TREE_TEST := $(wildcard $(TREE_PATH)/*.c)
TREE := $(filter-out $(TREE_PATH)/test.c, $(TREE_TEST))

SYMBOL := $(wildcard $(SYMBOL_PATH)/*.c)

SYMBOL_LIST_TEST := $(wildcard $(SYMBOL_LIST_PATH)/*.c)
SYMBOL_LIST := $(filter-out $(SYMBOL_LIST_PATH)/test.c, $(SYMBOL_LIST_TEST))

SYMBOL_TABLE_TEST := $(wildcard $(SYMBOL_TABLE_PATH)/*.c)
SYMBOL_TABLE := $(filter-out $(SYMBOL_TABLE_PATH)/test.c, $(SYMBOL_TABLE_TEST))

SYNTAX_TREE_TEST := $(wildcard $(SYNTAX_TREE_PATH)/*.c)
SYNTAX_TREE := $(filter-out $(SYNTAX_TREE_PATH)/test.c, $(SYNTAX_TREE_TEST))

# COMPILER INFRASTRUCTURE RULES

TARGETS := utils \
           list \
           stack \
           tree \
           symbol_list \
           symbol_table \
           syntax_tree \
           npc

utils: $(UTILS_TEST)
list: $(LIST_TEST)
tree: $(TREE_TEST)
stack: $(STACK_TEST) $(LIST)
symbol_list: $(SYMBOL_LIST_TEST) $(LIST) $(SYMBOL)
symbol_table: $(SYMBOL_TABLE_TEST) $(SYMBOL_LIST) $(SYMBOL) $(STACK) $(LIST)
syntax_tree: $(SYNTAX_TREE_TEST) $(SYMBOL) $(TREE)

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
